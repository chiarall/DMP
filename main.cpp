#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <thread>
#include <vector>
#include "DataParser.h"
#include <direct.h>

#define GetCurrentDir _getcwd

static const char *const POWER_SUPPLY_PATH = "cscript \"Driver\\PowerSupplyTEST.vbs\"";
static const char *const DATA_ACQUISITION_PATH = "cscript \"Driver\\dataAcquisitionTEST.vbs\"";
static const char *const ARDUINO_MOTOR_SHIELD = "cscript \"Driver\\motorController.vbs\"";

static const char *const BASE_PATH = "C:\\Users\\Belle2\\Desktop\\DMP_test\\";

DWORD WINAPI powerSupplyThread(LPVOID lpParam);

void setEnvVar(const char *envVar, const char *value);

void dataAcquisition(const char *nameDiamond, const char *voltage, const char *singleTime,
                     const char *totalAcquisitionTime);

void motorController(const char *speed, const char *numberOfStep);

void createVoltageFile(const char *path, const char *voltage, int channelStatus);

void goToNextVoltage(const char *path, int status);

const char *getFullPath(const char *pathCHzero, char *fileOutputPath);

using namespace std;

const wchar_t *GetWC(const char *c) {
    const size_t cSize = strlen(c) + 1;
    wchar_t *wc = new wchar_t[cSize];
    mbstowcs(wc, c, cSize);
    return wc;
}

void setEnvVar(const char *envVar, const char *value) {
    char buffer[100];
    strcpy(buffer, envVar);
    char *str1 = strcat(buffer, "=");
    char *str2 = strcat(str1, value);
    _wputenv(GetWC(str2));
    char *var = getenv(envVar);
    cout << envVar << " = " << var << endl;
}

void convertIntToString(int value, char *dest) {
    snprintf(dest, sizeof(dest), "%d", value);
}

int main() {
    const char *pathCHzero = "CH0.txt";
    const char *pathCHone = "CH1.txt";
    const char *pathCHtwo = "CH2.txt";
    const char *pathCHthree = "CH3.txt";
    const char *pathCNTRL = "CNTRL_ch.txt";
    HANDLE powerSupplyThreadHandle = 0;
    HANDLE arrayOfThread[1];

    cout << "porto la sorgente a finecorsa" << endl;
    motorController("1600", "80000");

    DataParser *parser = new DataParser("doeTable.dat");
    vector<vector<string>> doe = parser->read();

    // ************* Motor Parameter Control *******
    const char *speed = "1600";
    //const char *numberOfStep = "1600";
    int distance = -atoi(doe[0][5].c_str()) * 1600;
    char numberOfStep[100];
    convertIntToString(distance, numberOfStep);
    motorController(speed, numberOfStep);


    // ************* POWER SUPPLY *********
    const char *voltage = doe[0][2].c_str();
    const char *totalAcquisitionTime = doe[0][4].c_str();

    createVoltageFile(pathCHzero, voltage, 2);
    createVoltageFile(pathCHone, voltage, 2);
    createVoltageFile(pathCHtwo, voltage, 3);
    createVoltageFile(pathCHthree, voltage, 2);
    goToNextVoltage(pathCNTRL, 0);

    char fileOutputPath[FILENAME_MAX];
    setEnvVar("voltageValue", voltage);
    setEnvVar("pathCHzero", getFullPath(pathCHzero, fileOutputPath));
    setEnvVar("pathCHone", getFullPath(pathCHone,
                                       fileOutputPath)); // 2 corrisponde al canale 0, 3 corrisponde a CH1. 4 a CH2, 5 a CH3
    setEnvVar("pathCHtwo", getFullPath(pathCHtwo,
                                       fileOutputPath)); // 2 corrisponde al canale 0, 3 corrisponde a CH1. 4 a CH2, 5 a CH3
    setEnvVar("pathCHthree", getFullPath(pathCHthree,
                                         fileOutputPath)); // 2 corrisponde al canale 0, 3 corrisponde a CH1. 4 a CH2, 5 a CH3
    setEnvVar("pathCNTRL", getFullPath(pathCNTRL,
                                       fileOutputPath)); // 2 corrisponde al canale 0, 3 corrisponde a CH1. 4 a CH2, 5 a CH3

    powerSupplyThreadHandle = CreateThread(NULL, 0, powerSupplyThread, NULL, 0, NULL);
    if (powerSupplyThreadHandle == NULL) {
        ExitProcess(0);
    }

    Sleep(30000);

    // ************* DATA ACQUISITION LOOP ********
    for (int id = 0; id < doe.size(); id++) {
        if (id == 0) {
            cout << "Misura dell'offset. Coprire il diamante, premere il tasto \"y\" e invio per continuare." << endl;
            int stringaAttesa;
            cin >> stringaAttesa;
            setEnvVar("offsetSaveButton", "1");
            setEnvVar("offsetLoadButton", "0");
        } else {
            if (doe[id][6].compare(doe[id - 1][6]) != 0) {
                speed = "1600";
                int initialDistance = atoi(doe[id - 1][6].c_str());
                int finalDistance = atoi(doe[id][6].c_str());
                int distanceToMove = (initialDistance - finalDistance) * 1600;
                char nStep[100];
                convertIntToString(distanceToMove, nStep);
                motorController(speed, nStep);
            }
            setEnvVar("offsetSaveButton", "0");
            setEnvVar("offsetLoadButton", "1");
            voltage = doe[id][2].c_str();
            createVoltageFile(pathCHzero, voltage, 2);
            createVoltageFile(pathCHone, voltage, 2);
            createVoltageFile(pathCHtwo, voltage, 3);
            createVoltageFile(pathCHthree, voltage, 2);

            goToNextVoltage(pathCNTRL, 1); //la tensione viene cambiata con il valore successivo
            Sleep(20000);
        }

        // tempo della singola acquisizione in ms. Ogni punto � un valore medio su questo tempo
        // fare un ciclo for. Ad ogni ciclo i files con le tensioni vanno aggiornati dopo dataAcquisition e, ultimo passo, il file pathCNTRL va rimesso a 1 e termina con lo sleep
        const char *nameDiamond = doe[id][1].c_str();
        const char *singleTime = doe[id][3].c_str();
        dataAcquisition(nameDiamond, voltage, singleTime, totalAcquisitionTime);
    }

    // ************* SHUTDOWN POWERSUPPLY ********

    createVoltageFile(pathCHzero, voltage, 2);
    createVoltageFile(pathCHone, voltage, 2);
    createVoltageFile(pathCHtwo, voltage, 2);
    createVoltageFile(pathCHthree, voltage, 2);
    goToNextVoltage(pathCNTRL, 1); //la tensione viene cambiata con il valore successivo
    Sleep(20000);

    goToNextVoltage(pathCNTRL, 2);

    // mettere un 2 in pathCNTRL per terminare il powerSupply
    arrayOfThread[0] = powerSupplyThreadHandle;
    WaitForMultipleObjects(1, arrayOfThread, TRUE, INFINITE); // Attendo che il thread di powerSupply termini
    cout << "Digita q e premi invio per terminare." << endl;
    int stringaAttesa;
    cin >> stringaAttesa;
    return 0;
}

const char *getFullPath(const char *pathCHzero, char *fileOutputPath) {
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
        return "";
    }

    strcpy(fileOutputPath, cCurrentPath);
    strcat(fileOutputPath, "\\");
    strcat(fileOutputPath, pathCHzero);
    return fileOutputPath;
}

void createVoltageFile(const char *path, const char *voltage, int channelStatus) {
    ofstream file;
    file.open(path, std::ofstream::out | std::ofstream::trunc);
    if (!file) {
        cout << "file not found";
        return;
    }
    file << voltage << "\t5\t10\t10\t1000\t" << channelStatus;
    file.close();

}

void goToNextVoltage(const char *path, int status) {
    ofstream file;
    file.open(path, std::ofstream::out | std::ofstream::trunc);
    if (!file) {
        cout << "file not found";
        return;
    }
    file << status;
    file.close();
}


void dataAcquisition(const char *nameDiamond, const char *voltage, const char *singleTime,
                     const char *totalAcquisitionTime) {
    int time = atoi(totalAcquisitionTime);
    time = time * 1000; // tempo in ms
    int cycles = time / atoi(singleTime);
    cout << "Numero di cicli " << cycles << endl;


    char nCycles[100];
    convertIntToString(cycles, nCycles);

    char fileOutputPath[100];
    strcpy(fileOutputPath, BASE_PATH);
    strcat(fileOutputPath, nameDiamond);
    strcat(fileOutputPath, "_");
    strcat(fileOutputPath, voltage);
    strcat(fileOutputPath, "V");
    strcat(fileOutputPath, ".txt");

    cout << "fileOutputPath = " << fileOutputPath << endl;
    setEnvVar("filePath", fileOutputPath);
    setEnvVar("singleAcquisitionTime", singleTime);
    setEnvVar("numberOfCycles", nCycles);
    system(DATA_ACQUISITION_PATH); // Avvio acquisizione
}

void motorController(const char *speed, const char *numberOfStep) {
    setEnvVar("motorSpeed", speed);
    setEnvVar("numberOfStep", numberOfStep);
    system(ARDUINO_MOTOR_SHIELD); // Avvio PowerSupply
}


DWORD WINAPI powerSupplyThread(LPVOID lpParam) {
    system(POWER_SUPPLY_PATH); // Avvio PowerSupply
    return 0;
}
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <thread>


static const char *const POWER_SUPPLY_PATH = "cscript \"Driver\\PowerSupplyTEST.vbs\"";
static const char *const DATA_ACQUISITION_PATH = "cscript \"Driver\\dataAcquisitionTEST.vbs\"";
static const char *const ARDUINO_MOTOR_SHIELD = "cscript \"Driver\\motorController.vbs\"";

static const char *const BASE_PATH = "C:\\Users\\Belle2\\Desktop\\DMP_test\\";

DWORD WINAPI powerSupplyThread(LPVOID lpParam);

void setEnvVar(const char *envVar, const char *value);

// void powerSupply(const char *voltage, const char *onOffValue, const char *channel);

void dataAcquisition(const char *nameDiamond, const char *voltage, const char *singleTime,
                     const char *totalAcquisitionTime);

void motorController(const char *speed, const char *numberOfStep);

void createVoltageFile(const char *path, const char *voltage, int channelStatus);

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

    HANDLE powerSupplyThreadHandle = 0;
    HANDLE arrayOfThread[1];

    string ch;
    string infoFile[8];
    std::size_t found = std::string::npos;
    int i = 0;


    ifstream in;
    if (!in) {
        cout << "file not found";
        return -1;
    }
    in.open("fileInformazioniMisure.txt");
    while (!in.eof()) {
        in >> ch;
        if (found != std::string::npos) {
            infoFile[i] = ch;
            i++;
            found = std::string::npos;
        }
        found = ch.find(":");
    }
    in.close();

    for (int k = 0; k < 8; k++) {
        string test = infoFile[k];
        cout << "informazioni file di misura: " << infoFile[k] << endl;
        // infoFile[0] = Diamond name
        // infoFile[1] = Measurement type (stability, IV)
        // infoFile[2] = Diamond tension
        // infoFile[3] = min tension
        // infoFile[4] = max tension
        // infoFile[5] = single acquisition time
        // infoFile[6] = time
        // infoFile[7] = source distance
    }
/*
    // ************* Motor Parameter Control *******
    const char *speed = "1600";
    const char *numberOfStep = "1600";
    motorController(speed, numberOfStep);

*/
    // ************* POWER SUPPLY *********
    const char *voltage = infoFile[2].c_str();

    const char *pathCHzero = "CH0.txt";
    const char *pathCHone = "C:\\Users\\Belle2\\LABVIEW WORK\\TEST_DT5521\\CH1.txt";
    const char *pathCHtwo = "C:\\Users\\Belle2\\LABVIEW WORK\\TEST_DT5521\\CH2.txt";
    const char *pathCHthree = "C:\\Users\\Belle2\\LABVIEW WORK\\TEST_DT5521\\CH3.txt";
    const char *pathCNTRL = "C:\\Users\\Belle2\\LABVIEW WORK\\TEST_DT5521\\CNTRL_ch.txt";
    const char *totalAcquisitionTime = infoFile[6].c_str();

    createVoltageFile(pathCHzero, voltage, 1);

    setEnvVar("voltageValue", voltage);
    setEnvVar("pathCHzero", pathCHzero);
    setEnvVar("pathCHone", pathCHone); // 2 corrisponde al canale 0, 3 corrisponde a CH1. 4 a CH2, 5 a CH3
    setEnvVar("pathCHtwo", pathCHtwo); // 2 corrisponde al canale 0, 3 corrisponde a CH1. 4 a CH2, 5 a CH3
    setEnvVar("pathCHthree", pathCHthree); // 2 corrisponde al canale 0, 3 corrisponde a CH1. 4 a CH2, 5 a CH3
    setEnvVar("pathCNTRL", pathCNTRL); // 2 corrisponde al canale 0, 3 corrisponde a CH1. 4 a CH2, 5 a CH3
    powerSupplyThreadHandle = CreateThread(NULL, 0, powerSupplyThread, NULL, 0, NULL);
    if (powerSupplyThreadHandle == NULL) {
        ExitProcess(0);
    }

    Sleep(20000);
/*
    // ************* DATA ACQUISITION ********
    // tempo della singola acquisizione in ms. Ogni punto � un valore medio su questo tempo
    const char *nameDiamond = infoFile[0].c_str();
    const char *singleTime = infoFile[5].c_str();
    dataAcquisition(nameDiamond, voltage, singleTime, totalAcquisitionTime);


    arrayOfThread[0] = powerSupplyThreadHandle;
    WaitForMultipleObjects(1, arrayOfThread, TRUE, INFINITE); // Attendo che il thread di powerSupply termini

*/
    cout << "Digita q e premi invio per terminare." << endl;
    int stringaAttesa;
    cin >> stringaAttesa;
    return 0;
}

void createVoltageFile(const char *path, const char *voltage, int channelStatus) {
    ofstream file;
    file.open(path, std::ofstream::out | std::ofstream::trunc);
    if (!file) {
        cout << "file not found";
        return;
    }
    file << voltage << "\t20\t50\t50\t500\t" << channelStatus;
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

    setEnvVar("chOnValue", "1");
    system(POWER_SUPPLY_PATH); // Avvio PowerSupply

}

//void powerSupply(const char *voltage, const char *onOffValue, const char *channel) {
//    setEnvVar("voltageValue", voltage);
//    setEnvVar("chOnValue", onOffValue);
//    setEnvVar("chButton", channel); // 2 corrisponde al canale 0, 3 corrisponde a CH1. 4 a CH2, 5 a CH3
//    system(POWER_SUPPLY_PATH); // Avvio PowerSupply
//}

void motorController(const char *speed, const char *numberOfStep) {
    setEnvVar("motorSpeed", speed);
    setEnvVar("numberOfStep", numberOfStep);
    system(ARDUINO_MOTOR_SHIELD); // Avvio PowerSupply
}


DWORD WINAPI powerSupplyThread(LPVOID lpParam) {
    system(POWER_SUPPLY_PATH); // Avvio PowerSupply
    return 0;
}
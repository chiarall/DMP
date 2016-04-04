#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <thread>


static const char *const POWER_SUPPLY_PATH = "cscript \"Driver\\PowerSupplyTEST.vbs\"";
static const char *const DATA_ACQUISITION_PATH = "cscript \"Driver\\dataAcquisitionTEST.vbs\"";

void setEnvVar(const char *envVar, const char *value);

DWORD WINAPI startDataAcquisition(LPVOID lpParam);
DWORD WINAPI startPowerSupply(LPVOID lpParam);

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

    HANDLE handleOfPowerSupplyThread = 0;
    HANDLE handleOfDataAcquisitionThread = 0;
    HANDLE arrayOfThread[2];

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

    char fileOutputPath[100];

    const char *nameDiamond;
    nameDiamond = infoFile[0].c_str();

    const char *voltage = infoFile[2].c_str();
    setEnvVar("voltageValue", voltage);

    setEnvVar("chOnValue", "1");
    setEnvVar("chButton", "2"); // 2 corrisponde al canale 0, 3 corrisponde a CH1. 4 a CH2, 5 a CH3

    handleOfPowerSupplyThread = CreateThread(NULL, 0, startPowerSupply, NULL, 0, NULL);
    if (handleOfPowerSupplyThread == NULL) {
        ExitProcess(0);
    }

 /*   Sleep(30000);

    const char *singleTime; // tempo della singola acquisizione in ms. Ogni punto � un valore medio su questo tempo
    singleTime = infoFile[5].c_str();

    int time = atoi(infoFile[6].c_str());
    time = time * 1000; // tempo in ms
    int cycles = time / atoi(singleTime);
    cout << "Numero di cicli " << cycles << endl;
    strcpy(fileOutputPath, "C:\\Users\\Belle2\\Desktop\\DMP_test\\");
//    strcpy(fileOutputPath, "");
    strcat(fileOutputPath, nameDiamond);
    strcat(fileOutputPath, "_");
    strcat(fileOutputPath, voltage);
    strcat(fileOutputPath, "V");
    strcat(fileOutputPath, ".txt");

    cout << "fileOutputPath = " << fileOutputPath << endl;

    setEnvVar("filePath", fileOutputPath);

    setEnvVar("singleAcquisitionTime", singleTime);

    char nCycles[100];
    convertIntToString(cycles, nCycles);
    setEnvVar("numberOfCycles", nCycles);

    handleOfDataAcquisitionThread = CreateThread(NULL, 0, startDataAcquisition, NULL, 0, NULL);
    if (handleOfDataAcquisitionThread == NULL) {
        ExitProcess(0);
    }

    arrayOfThread[0] = handleOfPowerSupplyThread;
    arrayOfThread[1] = handleOfDataAcquisitionThread;

    WaitForMultipleObjects(2, arrayOfThread, TRUE, INFINITE );
*/
    cout << "Digita q e premi invio per terminare." << endl;
    int stringaAttesa;
    cin >> stringaAttesa;
    return 0;
}

DWORD WINAPI startDataAcquisition(LPVOID lpParam) {
    system(DATA_ACQUISITION_PATH); // Avvio acquisizione
    return 0;
}

DWORD WINAPI startPowerSupply(LPVOID lpParam) {
    system(POWER_SUPPLY_PATH); // Avvio PowerSupply
    return 0;
}

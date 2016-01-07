#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>

static const char *const POWER_SUPPLY_PATH = "cscript \"C:\\Users\\Chiara La Licata\\Desktop\\diamondMeasurementProgram\\PowerSupplyTEST.vbs\"";
static const char *const DATA_ACQUISITION_PATH = "cscript \"C:\\Users\\Chiara La Licata\\Desktop\\diamondMeasurementProgram\\dataAcquisitionTEST.vbs\"";


void setEnvVar(const char *envVar, const char *value);

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

    char fileOutputPath[100];
    const char *nameDiamond;

    nameDiamond = "DM";

    setEnvVar("voltageValue", "15");
    setEnvVar("chOnValue", "1");
    setEnvVar("chButton", "1");

    system(POWER_SUPPLY_PATH);

    int time = 100; // tempo acquisizione in secondi
    time = time * 1000; // tempo in ms
    int cycles = time / 2000;

    int singleTime; // tempo della singola acquisizione in ms. Ogni punto è un valore medio su questo tempo
    singleTime = 1000;


    strcpy(fileOutputPath, "C:\\Users\\Chiara La Licata\\Desktop\\");
    strcat(fileOutputPath, nameDiamond);
    strcat(fileOutputPath, ".txt");

    setEnvVar("filePath", fileOutputPath);

    char sTime[100];
    convertIntToString(singleTime, sTime);
    setEnvVar("singleAcquisitionTime", sTime);

    char nCycles[100];
    convertIntToString(cycles, nCycles);
    setEnvVar("numberOfCycles", nCycles);

    system(DATA_ACQUISITION_PATH);

    return 0;
}



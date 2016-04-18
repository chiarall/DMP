//
// Created by Chiara La Licata on 18/04/2016.
//

#include "DataParser.h"

using namespace std;

DataParser::DataParser(const char *path) {
    this->path = path;
}

vector<vector<string>> DataParser::read() {
    ifstream in;
    vector<vector<string>> doe;
    in.open(this->path);
    string idNumber, nameDiamond, voltage, samplingWindow, totalAcquisitionTime, sourceDistance;

    if (!in) {
        cout << "file not found";
        return doe;
    }
    string firstLine;
    getline(in, firstLine);
    while (!in.eof()) {
        in >> idNumber >> nameDiamond >> voltage >> samplingWindow >> totalAcquisitionTime >> sourceDistance;
        cout << "id = " << idNumber << "  name = " << nameDiamond << "  voltage = " << voltage <<
        "  sampling window = " << samplingWindow << "  total Acquisition = " << totalAcquisitionTime <<
        "  distance = " << sourceDistance << endl;

        vector<string> row;
        row.push_back(idNumber);
        row.push_back(nameDiamond);
        row.push_back(voltage);
        row.push_back(samplingWindow);
        row.push_back(totalAcquisitionTime);
        row.push_back(sourceDistance);

        doe.push_back(row);
    }
    in.close();
    return doe;
}


//
// Created by Chiara La Licata on 18/04/2016.
//

#include <sstream>
#include "DataParser.h"

using namespace std;

DataParser::DataParser(const char *path) {
    this->path = path;
}

vector<vector<string>> DataParser::read() {
    ifstream in;
    vector<vector<string>> doe;
    in.open(this->path);
    string idNumber, nameDiamond, voltage, samplingWindow, totalAcquisitionTime, sourceDistance, ch0Status, ch1Status, ch2Status, ch3Status;

    if (!in) {
        cout << "file not found";
        return doe;
    }
    string firstLine;
    getline(in, firstLine);

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);

        if (!(iss >> idNumber >> nameDiamond >> voltage >> samplingWindow >> totalAcquisitionTime >>
              sourceDistance >> ch0Status >> ch1Status >> ch2Status >> ch3Status)) {
            cout << "Impossibile leggere dal file!" << endl;
            break;
        } else {
            cout << "id = " << idNumber << "  name = " << nameDiamond << "  voltage = " << voltage <<
            "  sampling window = " << samplingWindow << "  total Acquisition = " << totalAcquisitionTime <<
            "  distance = " << sourceDistance << "  ch0Status =" << ch0Status << "  ch1Status =" << ch1Status <<
            "  ch2Status =" << ch2Status << "  ch3Status =" << ch3Status << endl;
            vector<string> row;
            row.push_back(idNumber);
            row.push_back(nameDiamond);
            row.push_back(voltage);
            row.push_back(samplingWindow);
            row.push_back(totalAcquisitionTime);
            row.push_back(sourceDistance);
            row.push_back(ch0Status.compare("OFF") == 0 ? "2" : "3");
            row.push_back(ch1Status.compare("OFF") == 0 ? "2" : "3");
            row.push_back(ch2Status.compare("OFF") == 0 ? "2" : "3");
            row.push_back(ch3Status.compare("OFF") == 0 ? "2" : "3");

            doe.push_back(row);
        }
    }

    in.close();
    return doe;
}


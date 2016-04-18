//
// Created by Chiara La Licata on 18/04/2016.
//

#ifndef RUNNER_DATAPARSER_H
#define RUNNER_DATAPARSER_H

#include <fstream>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <vector>


class DataParser {
    const char *path;
public:
    DataParser(const char *path);

    std::vector<std::vector<std::string>> read();


};


#endif //RUNNER_DATAPARSER_H

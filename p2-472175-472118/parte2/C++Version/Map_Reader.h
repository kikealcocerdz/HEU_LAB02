//
// Created by Administrador on 06/12/2023.
//

#ifndef CSPPARKING_PY_MAP_READER_H
#define CSPPARKING_PY_MAP_READER_H
#include "Ambulance.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

class Mapa {
public:
    std::vector<std::vector<char>> data;
    Mapa(std::string &file) {
        read_file(file);
    }
    void read_file(std::string &file);
    void print_info();
};


#endif //CSPPARKING_PY_MAP_READER_H

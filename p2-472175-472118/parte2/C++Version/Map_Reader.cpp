//
// Created by Administrador on 06/12/2023.
//

#include "Map_Reader.h"

//std::tuple<std::vector<char>, int>
void Mapa::read_file(std::string &file) {
    std::ifstream f(file);
    if (!f.is_open()) {
        throw std::runtime_error("Wrong path!!!!");
    }
    std::cout << "Reading file: " << file << std::endl;
    std::string line;
    while (std::getline(f, line)) {
        std::vector<char> tokens;
        std::string aux = "C";
        for (size_t i = 0; i < line.size(); i++) {
            // Check bounds
            char prev = (i > 0) ? line[i - 1] : '\0';
            char next = (i < line.size() - 1) ? line[i + 1] : '\0';

            if (prev == ';' && next != ';' && next != '\0') {
                aux += next;
                if (aux == "CN") {
                    tokens.push_back('W');
                    aux = "C";
                } else if (aux == "CC") {
                    tokens.push_back('E');
                    aux = "C";
                }
            } else if (prev == ';' || prev == '\0') {
                tokens.push_back(line[i]);
            }
        }
        data.push_back(tokens);
        // Debug print each parsed line
        for (char c : tokens) {
            std::cout << c << " ";
        }
        std::cout << "\n\n\n";
    }
}



void Mapa::print_info() {
    for (const auto &row: data) {
        for (const auto &cell: row) {
            std::cout << cell << " ";
        }
        std::cout << '\n';
    }
}


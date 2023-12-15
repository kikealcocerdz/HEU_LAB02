//
// Created by Administrador on 06/12/2023.
//

#ifndef CSPPARKING_PY_AMBULANCE_H
#define CSPPARKING_PY_AMBULANCE_H

#include "constants.h"

#include <iostream>
#include <map>
#include <tuple>
#include <vector>

struct temp_data {
    int priority;
    std::tuple<int, int> coordinates;
    std::array<int, 2> passengers;
    int n_delivered;
    int energy;
    std::vector<std::tuple<int, int>> n;
    std::vector<std::tuple<int, int>> c;
    int cost_til_now;
    std::vector<std::tuple<int, int>> path;
    std::tuple<int, int> next_objective;
    std::map<std::tuple<int, int>, std::array<int, 2>> crossed;

    // Constructor para facilitar la creación de instancias
    temp_data(int first, std::tuple<int, int> second, std::array<int, 2> third, int eleventh,
              int fourth, std::vector<std::tuple<int, int>> fifth,
              std::vector<std::tuple<int, int>> sixth, int seventh,
              std::vector<std::tuple<int, int>> eighth, std::tuple<int, int> ninth,
              std::map<std::tuple<int, int>, std::array<int, 2>> tenth)
      : priority(first), coordinates(second), passengers(third), n_delivered(eleventh),
        energy(fourth), n(fifth), c(sixth), cost_til_now(seventh), path(eighth),
        next_objective(ninth), crossed(tenth) { }
};

// Definir un comparador personalizado para la priority_queue
struct heap_order {
    bool operator()(temp_data const & lhs, temp_data const & rhs) const {
      // Comparar los primeros elementos de las estructuras
      return lhs.priority > rhs.priority;
    }
};

class Ambulance {
  public:
    // Constructor que toma un parámetro data
    Ambulance(std::vector<std::vector<char>> & data, int h) : h(h) { initializate_data(data); }

    char current_cost;
    int h;
    std::vector<std::tuple<int, int>> n;
    std::vector<std::tuple<int, int>> c;
    std::tuple<int, int> cc;
    std::tuple<int, int> cn;
    std::tuple<int, int> p;
    temp_data current_path = {
      0, p, {0, 0},
        0, max_energy, n, c, 0, {p},
        {},
        {}
    };

    // Función para inicializar los datos
    void initializate_data(std::vector<std::vector<char>> & data);
    void print_info();
};

#endif  // CSPPARKING_PY_AMBULANCE_H

//
// Created by Administrador on 06/12/2023.
//

#ifndef CSPPARKING_PY_A_STAR_H
#define CSPPARKING_PY_A_STAR_H

#include "Ambulance.h"
#include "Map_Reader.h"

#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

class A_star {
  public:
    A_star(Mapa & grid, Ambulance & amb) : map(grid), ambulance(amb) {
      // came_from[ambulance.p].push_back({});
      // draw_sol(a_star());
    }

    Mapa map;
    Ambulance ambulance;
    int expanded_counter = 0;

    // Variable to store a chrono

    int a_star();
    std::vector<std::tuple<int, int>> adyacents(std::tuple<int, int> & coordinates);
    int get_cost(char & node);
    // std::vector<std::vector<std::tuple<int, int>>> reconstruct_paths(std::tuple<int, int> &goal);
    int resolve_actions(char & type);
    void update_passengers();
    void draw_passed_tides(std::vector<std::tuple<int, int>> & paths);
    int calculate_heuristic(std::tuple<int, int> & pos, std::tuple<int, int> & next);
    std::tuple<int, int> find_nearest_passenger(std::tuple<int, int> & pos);
    // void draw_sol(std::vector<int> solution);
    bool create_and_write_file(Ambulance & ambulance);
};

#endif  // CSPPARKING_PY_A_STAR_H

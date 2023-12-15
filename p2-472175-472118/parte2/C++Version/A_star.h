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
  A_star(Mapa& grid, Ambulance& amb) : map(grid), ambulance(amb) {
    // came_from[ambulance.p].push_back({});
    // draw_sol(a_star());
  }

  Mapa map;
  Ambulance ambulance;
  std::string path_map = "";
  int heuristic_sel;
  int cost_til_now = 0;
  int path_length = 0;
  int expanded_counter = 0;
  std::chrono::high_resolution_clock::time_point time_start;  // Variable to store a chrono
  std::chrono::high_resolution_clock::time_point time_end;

  // Variable to store a chrono

  int a_star(std::string& path_map, int& heuristic_sel);
  std::vector<std::tuple<int, int>> adyacents(std::tuple<int, int>& coordinates);
  int get_cost(char& node);
  // std::vector<std::vector<std::tuple<int, int>>> reconstruct_paths(std::tuple<int, int> &goal);
  int resolve_actions(char& type);
  void update_passengers();
  void draw_passed_tides(std::vector<std::tuple<int, int>>& paths);
  int calculate_heuristic(std::tuple<int, int>& pos, std::tuple<int, int>& next);
  std::tuple<int, int> find_nearest_passenger(std::tuple<int, int>& pos);
  // void draw_sol(std::vector<int> solution);
  bool create_and_write_file(Ambulance& ambulance);
  void create_stats_file();
};

#endif  // CSPPARKING_PY_A_STAR_H

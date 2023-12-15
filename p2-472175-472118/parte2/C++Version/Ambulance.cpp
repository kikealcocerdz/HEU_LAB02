//
// Created by sergio on 7/12/23.
//
#include "Ambulance.h"

void Ambulance::initializate_data(std::vector<std::vector<char>> & data) {
  for (size_t i = 0; i < data.size(); i++) {
    std::vector<int> h_aux;
    for (size_t j = 0; j < data[i].size(); j++) {
      if (std::find(types.begin(), types.end(), data[i][j]) != types.end()) {
        if (data[i][j] == 'N') {
          n.push_back({i, j});
        } else if (data[i][j] == 'C') {
          c.push_back({i, j});
        } else if (data[i][j] == 'W') {
          cn = std::make_tuple(i, j);
        } else if (data[i][j] == 'E') {
          cc = std::make_tuple(i, j);
        } else if (data[i][j] == 'P') {
          p = std::make_tuple(i, j);
        }
      }
    }
  }
  current_path = {
    0, p, {0, 0},
      0, max_energy, n, c, 0, {p},
      {},
      {}
  };
}

void Ambulance::print_info() {
  std::cout << "Prioridad: " << current_path.priority << "\t Posición: ("
            << std::get<0>(current_path.coordinates) << " , "
            << std::get<1>(current_path.coordinates) << ")\t N_enfermos: ["
            << current_path.passengers.at(0) << " , " << current_path.passengers.at(1)
            << "]\t Energía: " << current_path.energy << "\t N: ";
  for (auto & elem : current_path.n) {
    std::cout << "[" << std::get<0>(elem) << " , " << std::get<1>(elem) << "]\t";
  }
  std::cout << "\t C: ";
  for (auto & elem : current_path.c) {
    std::cout << "[" << std::get<0>(elem) << " , " << std::get<1>(elem) << "]\t";
  }
  std::cout << "\t Cost til now: " << current_path.cost_til_now << "\n Path: ";
  for (auto & elem : current_path.path) {
    std::cout << "[" << std::get<0>(elem) << " , " << std::get<1>(elem) << "]\t";
  }
  std::cout << '\n';
  std::cout << "P: " << std::get<0>(p) << std::get<1>(p) << "\t";
  std::cout << "CN: " << std::get<0>(cc) << std::get<1>(cc) << "\t";
  std::cout << "CC: " << std::get<0>(cn) << std::get<1>(cn) << "\n";
  std::cout << "N: [";
  for (auto & sick : n) {
    std::cout << "[" << std::get<0>(sick) << ", " << std::get<1>(sick) << "]\t";
  }
  std::cout << "]\nC: [";
  for (auto & sick : c) {
    std::cout << "[" << std::get<0>(sick) << ", " << std::get<1>(sick) << "]\t";
  }
  std::cout << "]\n";
}

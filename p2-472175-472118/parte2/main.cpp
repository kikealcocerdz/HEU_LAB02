#include "C++Version/A_star.h"
#include "C++Version/Ambulance.h"
#include "C++Version/Map_Reader.h"

#include <iostream>
#include <string>
#include <chrono>

std::string path_map;
int heuristic_sel;

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Numero de argumentos Mal" << argv;
    return -1;
  }
  path_map = argv[1];
  std::cout << "Mapa: " << path_map << std::endl;
  heuristic_sel = atoi(argv[2]);
  Mapa mapa = Mapa(path_map);
  mapa.print_info();
  Ambulance ambulance = Ambulance(mapa.data, *argv[2]);
  // ambulance.print_info();
  A_star search = A_star(mapa, ambulance);
  // Start the timer 
  int result = search.a_star(path_map, heuristic_sel);
  // Stop the timer
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(search.time_end - search.time_start);
  // Write the stats file, with the name map_name_stats.txt
  search.create_stats_file();
  
  std::cout << "Tiempo total: " << duration.count() / 1000000 << " seconds with an ouput code: " << result << std::endl;
  return 0;
}

#include "C++Version/A_star.h"
#include "C++Version/Ambulance.h"
#include "C++Version/Map_Reader.h"

#include <iostream>

int main(int argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "Numero de argumentos Mal" << argv;
    return -1;
  }
  Mapa mapa = Mapa(reinterpret_cast<std::string &>(argv[1]));
  mapa.print_info();
  Ambulance ambulance = Ambulance(mapa.data, *argv[2]);
  // ambulance.print_info();
  A_star search = A_star(mapa, ambulance);
  return search.a_star();
}

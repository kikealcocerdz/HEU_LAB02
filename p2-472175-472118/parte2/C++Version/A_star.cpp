//
// Created by Administrador on 06/12/2023.
//

#include "A_star.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

// std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>> came_from;

std::vector<std::tuple<int, int>> A_star::adyacents(std::tuple<int, int>& coordinates)
{
  int const row = std::get<0>(coordinates);
  int const col = std::get<1>(coordinates);
  std::vector<std::tuple<int, int>> neighbors = {
      {row - 1, col},
      {row + 1, col},
      {row, col - 1},
      {row, col + 1} };
  std::vector<std::tuple<int, int>> result;
  for (auto& neighbor : neighbors)
  {
    size_t const r = std::get<0>(neighbor);
    size_t const c = std::get<1>(neighbor);
    // if ((0 <= r && r < map.data.size()) && (0 <= c && c < map.data[0].size()))
    if (r < map.data.size() && c < map.data[0].size())
    {
      result.push_back({ r, c });
    }
  }
  /*
  std::cout << row << col << '\n';
  for (auto &elem: result){std::cout << std::get<0>(elem) << std::get<1>(elem) << '\t';}
  std::cout << '\n';
   */
  return result;
}

int A_star::get_cost(char& node)
{
  if (isdigit(node))
  {
    return node - '0';
  }
  if (node == 'X')
  {
    return 1410065407;
  }
  return 1;
}

int A_star::resolve_actions(char& type)
{
  if (type == 'N' || type == 'C')
  {
    update_passengers();
    // CN es W y CC es E
  }
  else if (type == 'W' || type == 'E')
  {
    // Variables auxiliares para que sea mas facil referenciar
    // non contagious
    auto& non_cont = ambulance.current_path.passengers.at(0);
    // Contagious
    auto& cont = ambulance.current_path.passengers.at(1);
    if (type == 'W')
    {
      if (cont <= 0)
      {
        if (non_cont)
        {
          non_cont = 0;
          ambulance.current_path.n_delivered += 1;
        }
        if (cont < 0)
        {
          cont = 0;
        }
      }
    }
    else if (type == 'E')
    {
      if (cont > 0)
      {
        cont = 0;
        ambulance.current_path.n_delivered += 1;
      }
    }
  }
  if (ambulance.current_path.n.empty() && ambulance.current_path.c.empty() &&
    ambulance.current_path.coordinates == ambulance.p)
  {
    // Stop the clock
    std::array<int, 2> const comp = { 0, 0 };
    if (ambulance.current_path.passengers == comp)
    {
      draw_passed_tides(ambulance.current_path.path);
      this->time_end = std::chrono::high_resolution_clock::now();
      bool check = create_and_write_file(ambulance);
      if (!check)
      {
        std::cout << "Error creating the file\n";
      }
      return 1000;
    }
    // ambulance.print_info();
    // draw_passed_tides(ambulance.current_path.path);
  }
  ambulance.current_path.next_objective =
    find_nearest_passenger(ambulance.current_path.coordinates);

  return 0;
}

std::tuple<int, int> A_star::find_nearest_passenger(std::tuple<int, int>& pos)
{
  std::vector<std::tuple<int, int>> all_patients;
  for (auto& non_cont : ambulance.current_path.n)
  {
    all_patients.push_back(non_cont);
  }
  for (auto& cont : ambulance.current_path.c)
  {
    all_patients.push_back(cont);
  }
  std::tuple<int, int> sol = { -1, -1 };
  if (!all_patients.empty())
  {
    int minimum = 99999;
    for (auto& elem : all_patients)
    {
      int possible = calculate_heuristic(pos, elem);
      minimum = std::min(minimum, possible);
      if (minimum == possible)
      {
        sol = elem;
      }
    }
  }
  return sol;
}

void A_star::update_passengers()
{
  bool takenn = false;
  bool takenc = false;
  // Variables auxiliares para que sea mas facil referenciar
  // non contagious
  int& non_cont = ambulance.current_path.passengers.at(0);
  // Contagious
  int& cont = ambulance.current_path.passengers.at(1);
  if (ambulance.current_cost == 'N')
  {
    if (non_cont < non_contagious_seats)
    {
      non_cont += 1;
      takenn = true;
    }
    else if (-2 < cont && cont <= 0)
    {
      cont -= 1;
      takenc = true;
    }
  }
  else if (ambulance.current_cost == 'C')
  {
    if (0 <= cont && cont < contagious_seats)
    {
      cont += 1;
      takenc = true;
    }
  }
  if (takenn)
  {
    for (size_t i = 0; i < ambulance.current_path.n.size(); i++)
    {
      if (ambulance.current_path.n[i] == ambulance.current_path.coordinates)
      {
        ambulance.current_path.n.erase(ambulance.current_path.n.begin() + i);
      }
    }
  }
  if (takenc)
  {
    for (size_t i = 0; i < ambulance.current_path.c.size(); i++)
    {
      if (ambulance.current_path.c[i] == ambulance.current_path.coordinates)
      {
        ambulance.current_path.c.erase(ambulance.current_path.c.begin() + i);
      }
    }
  }
  // draw_passed_tides(ambulance.current_path.path);
}

int A_star::a_star(std::string& path_map, int& heuristic_sel)
{
  this->time_start = std::chrono::high_resolution_clock::now();
  this->path_map = path_map;
  this->heuristic_sel = heuristic_sel;
  std::priority_queue<temp_data, std::vector<temp_data>, heap_order> open_set;
  open_set.push(ambulance.current_path);
  while (!open_set.empty())
  {
    ambulance.current_path = open_set.top();
    // ambulance.print_info();
    //  Lo borramos
    open_set.pop();
    expanded_counter += 1;
    // draw_passed_tides(ambulance.current_path.path);

    // Variable auxiliar, para simplificar
    int const& coordinate0 = std::get<0>(ambulance.current_path.coordinates);
    int const& coordinate1 = std::get<1>(ambulance.current_path.coordinates);
    ambulance.current_cost = map.data[coordinate0][coordinate1];

    // std::cout << std::get<3>(ambulance.current_path) << '\n';
    // std::cout << ambulance.current_cost << '\n';
    ambulance.current_path.energy -= get_cost(ambulance.current_cost);

    if (ambulance.current_path.coordinates == ambulance.p)
    {
      ambulance.current_path.energy = max_energy;
      ambulance.current_path.next_objective =
        find_nearest_passenger(ambulance.current_path.coordinates);
    }
    if (std::find(types.begin(), types.end(), map.data[coordinate0][coordinate1]) != types.end())
    {
      char& current_type = map.data[coordinate0][coordinate1];
      int posible_sol = resolve_actions(current_type);
      if (posible_sol == 1000)
      {
        return 0;
      }
    }
    if (ambulance.current_path.n.empty() && ambulance.current_path.c.empty())
    {
      std::array<int, 2> const comp = { 0, 0 };
      if (ambulance.current_path.passengers == comp)
      {
        ambulance.current_path.next_objective = ambulance.p;
      }
      else if (ambulance.current_path.passengers.at(1) > 0)
      {
        ambulance.current_path.next_objective = ambulance.cc;
      }
      else if (ambulance.current_path.passengers.at(0) > 0 ||
        ambulance.current_path.passengers.at(1) < 0)
      {
        ambulance.current_path.next_objective = ambulance.cn;
      }
    }

    for (auto& neighbor : adyacents(ambulance.current_path.coordinates))
    {
      auto n_pos = map.data[std::get<0>(neighbor)][std::get<1>(neighbor)];
      int new_cost = get_cost(n_pos);
      int n_delivered = ambulance.current_path.n_delivered;
      auto find_n =
        std::find(ambulance.current_path.n.begin(), ambulance.current_path.n.end(), neighbor);
      auto find_c =
        std::find(ambulance.current_path.c.begin(), ambulance.current_path.c.end(), neighbor);

      if (find_n != ambulance.current_path.n.end() || find_c != ambulance.current_path.c.end())
      {
        n_delivered += 1;
      }
      if (0 < new_cost && new_cost < 1410065407)
      {
        bool push = false;
        auto found = ambulance.current_path.crossed.find(neighbor);
        if (found != ambulance.current_path.crossed.end())
        {
          if (n_delivered > found->second.at(1))
          {
            ambulance.current_path.crossed[neighbor] = { new_cost, n_delivered };
            push = true;
          }
          else if (ambulance.current_path.energy < found->second.at(0) &&
            n_delivered <= found->second.at(1))
          {
            ambulance.current_path.crossed[neighbor] = { new_cost, n_delivered };
            push = true;
            // draw_passed_tides(ambulance.current_path.path);
          }
        }
        else
        {
          ambulance.current_path.crossed[neighbor] = { new_cost, n_delivered };
          push = true;
        }
        if (push)
        {
          auto new_heuristic = calculate_heuristic(neighbor, ambulance.current_path.next_objective);

          new_cost += ambulance.current_path.cost_til_now;
          int priority = new_cost - n_delivered * 4 + new_heuristic;
          std::vector<std::tuple<int, int>> newpath = ambulance.current_path.path;
          newpath.push_back(neighbor);
          temp_data new_n = {
              priority,
              neighbor,
              ambulance.current_path.passengers,
              n_delivered,
              ambulance.current_path.energy,
              ambulance.current_path.n,
              ambulance.current_path.c,
              new_cost,
              newpath,
              ambulance.current_path.next_objective,
              ambulance.current_path.crossed,
          };
          open_set.push(new_n);
        }
      }
    }
    // ambulance.print_info();
    /*
    for (auto &elem: crossed) {
      std::cout << "("<< std::get<0>(elem.first) << "," << std::get<1>(elem.first) <<
          ")->["<< elem.second.at(0) << "," << elem.second.at(1) << "]\t";
    }
    std::cout << '\n';
     */
  }

  // draw_passed_tides(ambulance.current_path.path);
  std::cout << "No possible path!!!\n";
  return -1;
}

bool A_star::create_and_write_file(Ambulance& ambulance)
{
  try
  {
    std::string file_name = this->path_map.substr(0, path_map.length() - 4) + "-" + std::to_string(this->heuristic_sel) + ".output";
    std::ofstream file(file_name); // Open the file

    if (!file.is_open())
    {
      throw std::runtime_error("Error opening file");
    }

    int accumulated_cost = 0;

    for (auto const& j : ambulance.current_path.path)
    {
      char state = map.data[std::get<0>(j)][std::get<1>(j)];
      std::string line;

      switch (state)
      {
      case 'W':
      {
        line = "(" + std::to_string(std::get<0>(j)) + ", " + std::to_string(std::get<1>(j)) +
          "): CN" + ": ";
        break;
      }
      case 'E':
      {
        line = "(" + std::to_string(std::get<0>(j)) + ", " + std::to_string(std::get<1>(j)) +
          "): CC" + ": ";
        break;
      }
      default:
      {
        line = "(" + std::to_string(std::get<0>(j)) + ", " + std::to_string(std::get<1>(j)) +
          "): " + state + ": ";
        break;
      }
      }

      accumulated_cost += get_cost(state);
      std::cout << line << 50 - accumulated_cost + 1 << std::endl;
      file << line << 50 - accumulated_cost + 1 << std::endl;
    }
    file.close(); // Close the file

    // Write the stats file, with the name map_name_stats.txt
    std::string stats_file_name = this->path_map.substr(0, path_map.length() - 4) + "-" + std::to_string(this->heuristic_sel) + ".stats";
    std::ofstream stats_file(stats_file_name); // Open the file
    /*
    The format of the file is:
    Tiempo total: 145
    Coste total: 38
    Longitud del plan: 27
    Nodos expandidos: 132
    */
    if (!stats_file.is_open())
    {
      throw std::runtime_error("Error opening file");
    }

    this->cost_til_now = ambulance.current_path.cost_til_now;
    this->path_length = ambulance.current_path.path.size();

    stats_file << "Tiempo total: " << std::chrono::duration_cast<std::chrono::microseconds>(this->time_end - this->time_start).count() / 1000000 << "\n";
    stats_file << "Coste total: " << this->cost_til_now << "\n";
    stats_file << "Longitud del plan: " << this->path_length << "\n";
    stats_file << "Nodos expandidos: " << this->expanded_counter << "\n";

    return true;
  }
  catch (std::exception const& e)
  {
    std::cerr << "ERROR CREATING THE FILE: " << e.what() << std::endl;
    return false;
  }
};

int A_star::calculate_heuristic(std::tuple<int, int>& pos, std::tuple<int, int>& next)
{
  if (ambulance.h == '1')
  {
    return std::abs(std::get<0>(pos) - std::get<0>(next)) +
      std::abs(std::get<1>(pos) - std::get<1>(next));
  }
  else if (ambulance.h == '2')
  {
    // Definir segunda heuristica
    return 0;
  }
  return -1;
}

void A_star::draw_passed_tides(std::vector<std::tuple<int, int>>& path)
{
  std::string draw = "";
  for (size_t i = 0; i < map.data.size(); i++)
  {
    for (size_t j = 0; j < map.data[0].size(); j++)
    {
      std::tuple<int, int> const tuple = { i, j };
      auto it = std::find(path.begin(), path.end(), tuple);
      if (it != path.end())
      {
        // Calcular el índice de la posición en path
        int index = std::distance(path.begin(), it);
        draw += std::to_string(index) + "\t\t";
      }
      else
      {
        draw += "-\t\t";
      }
    }
    draw += "\n";
  }
  std::cout << draw << '\n';
}

void A_star::create_stats_file() {

}

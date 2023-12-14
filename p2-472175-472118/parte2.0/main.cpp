#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <map>
#include <queue>
#include <utility>

// Assuming global variables
std::vector<std::vector<std::string>> data;
std::vector<std::pair<int, int>> N, C, P, CC, CN;
int max_energy, non_contagious_seats, contagious_seats;

// Function declarations
std::vector<std::vector<std::string>> read_file(const std::string& file);

// Other function and class declarations will go here
class Ambulancia {
public:
    std::pair<int, int> position;
    int energy;
    std::vector<int> passengers; // Assuming format [contagious, non-contagious]
    std::vector<std::pair<int, int>> no_contagious_left, contagious_left;
    std::vector<std::pair<int, int>> path;
    std::map<std::pair<int, int>, std::vector<int>> closed_dict; // Assuming format {position: [delivered, cost]}
    std::pair<int, int> next_objective;
    int cost_til_now;
    int number_of_delivered;
    int aproximate_cost_to_objective;

    Ambulancia(std::pair<int, int> pos, int en = 50, std::vector<int> pass = {0, 0},
               std::vector<std::pair<int, int>> non_cont = {}, std::vector<std::pair<int, int>> cont = {},
               std::vector<std::pair<int, int>> p = {}, std::map<std::pair<int, int>, std::vector<int>> closed = {},
               std::pair<int, int> n_obj = {}, int cost_now = 0, int num = 0, int c_obj = 0)
            : position(pos), energy(en), passengers(std::move(pass)), no_contagious_left(std::move(non_cont)), contagious_left(std::move(cont)),
              path(std::move(p)), closed_dict(std::move(closed)), next_objective(n_obj), cost_til_now(cost_now),
              number_of_delivered(num), aproximate_cost_to_objective(c_obj) {}

    bool operator<(const Ambulancia& other) const {
        if (number_of_delivered > other.number_of_delivered) {
            return true;
        } else if (number_of_delivered == other.number_of_delivered) {
            if (aproximate_cost_to_objective < other.aproximate_cost_to_objective) {
                return true;
            } else if (aproximate_cost_to_objective == other.aproximate_cost_to_objective) {
                return energy > other.energy;
            }
        }
        return false;
    }

    bool operator>(const Ambulancia& other) const {
        if (number_of_delivered < other.number_of_delivered) {
            return true;
        } else if (number_of_delivered == other.number_of_delivered) {
            if (aproximate_cost_to_objective > other.aproximate_cost_to_objective) {
                return true;
            } else if (aproximate_cost_to_objective == other.aproximate_cost_to_objective) {
                return energy < other.energy;
            }
        }
        return false;
    }
};

class A_star {
public:
    Ambulancia ambulance;
    int h;

    A_star(Ambulancia amb, int heur) : ambulance(amb), h(heur) {
        a_star();
    }

    std::vector<std::pair<int, int>> adyacents() {
        int row = ambulance.position.first, col = ambulance.position.second;
        std::vector<std::pair<int, int>> neighbors = {{row - 1, col}, {row + 1, col}, {row, col - 1}, {row, col + 1}};
        std::vector<std::pair<int, int>> valid_neighbors;
        for (auto &n : neighbors) {
            if (0 <= n.first && n.first < data.size() && 0 <= n.second && n.second < data[0].size()) {
                valid_neighbors.push_back(n);
            }
        }
        return valid_neighbors;
    }

    int get_cost(const std::pair<int, int>& node) {
        std::string node_value = data[node.first][node.second];
        if (isdigit(node_value[0])) {
            return std::stoi(node_value);
        } else if (node_value == "X") {
            return -1;
        }
        return 1;
    }


    void draw_path() {
        std::string draw;
        for (int i = 0; i < data.size(); ++i) {
            for (int j = 0; j < data[0].size(); ++j) {
                if (j % data[0].size() == 0) {
                    draw += "\n";
                }
                if (std::find(ambulance.path.begin(), ambulance.path.end(), std::make_pair(i, j)) != ambulance.path.end()) {
                    draw += std::to_string(std::distance(ambulance.path.begin(), std::find(ambulance.path.begin(), ambulance.path.end(), std::make_pair(i, j)))) + "\t\t";
                } else {
                    draw += "-\t\t";
                }
            }
        }
        std::cout << draw << std::endl;
        for (auto &p : ambulance.path) {
            std::cout << "(" << p.first << ", " << p.second << ") ";
        }
        std::cout << std::endl;
    }


    int heuristic(const std::pair<int, int>& position, const std::pair<int, int>& new_pos) {
        if (h == 1) {
            return std::abs(new_pos.first - position.first) + std::abs(new_pos.second - position.second);
        }
        // Additional heuristic calculations can be added here
        return 0; // Default return if no heuristic matches
    }

    std::pair<int, int> nearest_passenger(bool isContagious, bool &found) {
        found = false;
        int minimumDistance = std::numeric_limits<int>::max();
        std::pair<int, int> nearest;

        const auto &passengerList = isContagious ? ambulance.contagious_left : ambulance.no_contagious_left;

        for (const auto &passenger : passengerList) {
            int distance = heuristic(ambulance.position, passenger);
            if (distance < minimumDistance) {
                minimumDistance = distance;
                nearest = passenger;
                found = true;
            }
        }

        return nearest;
    }



    bool resolve_actions() {
        if (ambulance.position == P[0]) {
            ambulance.energy = max_energy;
            if (ambulance.no_contagious_left.empty() && ambulance.contagious_left.empty() &&
                ambulance.passengers[0] == 0 && ambulance.passengers[1] == 0) {
                draw_path();
                return true; // Path found
            }
        }

        if (ambulance.passengers[1] == non_contagious_seats && std::abs(ambulance.passengers[0]) == contagious_seats) {
            ambulance.next_objective = CC[0];
        } else {
            bool n_c_found = false, n_n_found = false;
            std::pair<int, int> n_c = nearest_passenger(true, n_c_found);
            std::pair<int, int> n_n = nearest_passenger(false, n_n_found);

            if (ambulance.passengers[0] >= 1 && ambulance.passengers[0] < 2) {
                if (n_c_found) {
                    if (heuristic(ambulance.position, n_c) <= heuristic(ambulance.position, CC[0])) {
                        ambulance.next_objective = n_c;
                    }
                } else {
                    ambulance.next_objective = CC[0];
                }
            } else {
                int distance_nc = std::numeric_limits<int>::max();
                int distance_nn = std::numeric_limits<int>::max();
                if (n_c_found) {
                    distance_nc = heuristic(ambulance.position, n_c);
                }
                if (n_n_found) {
                    distance_nn = heuristic(ambulance.position, n_n);
                }

                if (distance_nn != distance_nc) {
                    int m = std::min(distance_nc, distance_nn);
                    if (m == distance_nn) {
                        if (ambulance.passengers[0] >= 1 && ambulance.passengers[0] < 2) {
                            ambulance.next_objective = CC[0];
                        } else if (ambulance.passengers[1] > 0 && ambulance.passengers[1] < non_contagious_seats ||
                                   ambulance.passengers[0] <= 0 && ambulance.passengers[0] > -2) {
                            if (distance_nn <= heuristic(ambulance.position, CN[0])) {
                                ambulance.next_objective = n_n;
                            } else {
                                ambulance.next_objective = CN[0];
                            }
                        } else if (ambulance.passengers[1] == 0) {
                            ambulance.next_objective = n_n;
                        }
                    } else if (m == distance_nc) {
                        ambulance.next_objective = n_c;
                    }
                }
            }
        }

        return false; // No action resolved
    }

    void a_star() {
        std::priority_queue<Ambulancia, std::vector<Ambulancia>, std::greater<Ambulancia>> open_set;
        open_set.push(ambulance);

        while (!open_set.empty()) {
            Ambulancia current = open_set.top();
            open_set.pop();

            // If the goal is reached, construct and return the path
            // This part depends on your specific goal condition

            // Get the cost of moving to the current position
            int move_cost = get_cost(current.position);
            if (move_cost > 0) {
                // Update the ambulance state based on the current position
                // This might involve updating the path, energy, etc.

                // Check if we need to perform any actions based on the current type
                std::string current_type = data[current.position.first][current.position.second];
                if (std::find(types.begin(), types.end(), current_type) != types.end()) {
                    resolve_passengers(current_type);
                }

                // Resolve actions and check for the end condition
                if (resolve_actions()) {
                    break; // Path found or goal reached
                }

                // Explore adjacent nodes
                for (const auto& next : adyacents(current.position)) {
                    // Compute the cost for the next node and update the path if it's a better path
                    // Here, use the heuristic function to estimate the cost to the goal from 'next'
                    // Add the updated ambulance state to the open set if it's a valid move
                }
            }
        }

        // Handle the case when no path is found or the goal is not reachable
        std::cout << "No solution found" << std::endl;
    }




};

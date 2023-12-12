#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

struct Node {
    int x, y;       // Position on the grid
    int G, H;       // G: Cost from start, H: Heuristic cost to end
    Node *parent;   // Parent node in the path

    Node(int x, int y) : x(x), y(y), G(0), H(0), parent(nullptr) {}

    int getF() const { return G + H; } // F is the total cost (G + H)

    // Overload the < operator to use the node in a priority queue
    bool operator<(const Node& other) const {
        return getF() > other.getF();
    }
};

// Calculate Manhattan Distance as the heuristic
int calculateHeuristic(Node* a, Node* b) {
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

// Function to find the path by backtracking from goal to start
std::vector<Node*> findPath(Node* start, Node* goal) {
    std::vector<Node*> path;
    for (Node* node = goal; node != nullptr; node = node->parent) {
        path.push_back(node);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int main() {
    // Define the grid (0 = free, 1 = obstacle)
    std::vector<std::vector<int>> grid = {
            {0, 0, 0, 0, 1},
            {0, 1, 1, 0, 0},
            {0, 0, 0, 1, 0},
            {1, 0, 0, 0, 0}
    };

    // Initialize the start and goal nodes
    Node* start = new Node(0, 0); // Replace with your start coordinates
    Node* goal = new Node(3, 4);  // Replace with your goal coordinates

    std::priority_queue<Node> openList;
    std::vector<Node*> closedList;
    openList.push(*start);

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

        if (current.x == goal->x && current.y == goal->y) {
            // Goal found, backtrack to get the path
            std::vector<Node*> path = findPath(start, &current);
            // Print the path or handle it as needed
            for (auto node : path) {
                std::cout << "(" << node->x << ", " << node->y << ") ";
            }
            std::cout << std::endl;
            break;
        }

        closedList.push_back(new Node(current.x, current.y));

        // Process neighbors
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                if (dx != 0 && dy != 0) continue; // Skip diagonals

                int newX = current.x + dx;
                int newY = current.y + dy;

                if (newX < 0 || newX >= grid.size() || newY < 0 || newY >= grid[0].size() || grid[newX][newY] == 1) {
                    continue;
                }

                Node* neighbor = new Node(newX, newY);
                neighbor->G = current.G + 1;
                neighbor->H = calculateHeuristic(neighbor, goal);
                neighbor->parent = new Node(current.x, current.y);

                if (std::find_if(closedList.begin(), closedList.end(),
                                 [newX, newY](const Node* n) { return n->x == newX && n->y == newY; }) == closedList.end()) {
                    openList.push(*neighbor);
                }
            }
        }
    }

    return 0;
}

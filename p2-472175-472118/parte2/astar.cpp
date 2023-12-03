#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

struct Node {
    int x, y;  // Coordinates of the node
    int g, h;  // Cost from the start node (g) and heuristic cost to the goal (h)
    Node* parent;  // Parent node for path reconstruction

    Node(int x, int y, int g, int h, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), parent(parent) {}

    int f() const {
        return g + h;  // Total cost (g + h)
    }

    bool operator<(const Node& other) const {
        return f() > other.f();  // Priority queue uses > for min heap
    }
};

// Function to calculate the Manhattan distance heuristic
int calculateHeuristic(int currentX, int currentY, int goalX, int goalY) {
    return abs(currentX - goalX) + abs(currentY - goalY);
}

// A* algorithm implementation with step costs
vector<pair<int, int>> astar(const vector<vector<int>>& grid, const vector<vector<int>>& stepCosts, pair<int, int> start, pair<int, int> goal) {
    priority_queue<Node> openSet;
    vector<vector<bool>> closedSet(grid.size(), vector<bool>(grid[0].size(), false));

    openSet.push({start.first, start.second, 0, calculateHeuristic(start.first, start.second, goal.first, goal.second)});

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.x == goal.first && current.y == goal.second) {
            // Reconstruct the path
            vector<pair<int, int>> path;
            while (current.parent) {
                path.push_back({current.x, current.y});
                current = *current.parent;
            }
            path.push_back({start.first, start.second});
            reverse(path.begin(), path.end());
            return path;
        }

        closedSet[current.x][current.y] = true;

        // Define possible movements (up, down, left, right)
        vector<pair<int, int>> movements = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (const auto& move : movements) {
            int newX = current.x + move.first;
            int newY = current.y + move.second;

            if (newX >= 0 && newX < grid.size() && newY >= 0 && newY < grid[0].size() && !closedSet[newX][newY] && grid[newX][newY] == 0) {
                int newG = current.g + stepCosts[newX][newY];  // Use step cost from the matrix
                int newH = calculateHeuristic(newX, newY, goal.first, goal.second);
                openSet.push({newX, newY, newG, newH, &current});
                closedSet[newX][newY] = true;
            }
        }
    }

    // No path found
    return {};
}

int main() {
    // Example usage with step costs
    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}
    };

    vector<vector<int>> stepCosts = {
        {1, 1, 1, 1, 1},
        {1, 2, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 2, 1},
        {1, 1, 1, 1, 1}
    };

    pair<int, int> start = {0, 0};
    pair<int, int> goal = {4, 4};

    vector<pair<int, int>> path = astar(grid, stepCosts, start, goal);

    if (path.empty()) {
        cout << "No path found." << endl;
    } else {
        cout << "Path found:" << endl;
        for (const auto& point : path) {
            cout << "(" << point.first << ", " << point.second << ") ";
        }
        cout << endl;
    }

    return 0;
}

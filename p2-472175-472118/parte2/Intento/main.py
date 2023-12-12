import heapq


class Node:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.G = 0  # Cost from start to current node
        self.H = 0  # Heuristic cost from current node to end
        self.parent = None

    def __lt__(self, other):
        return self.G + self.H < other.G + other.H


def calculate_heuristic(a, b):
    return abs(a.x - b.x) + abs(a.y - b.y)


def find_path(start, goal):
    path = []
    current = goal
    while current is not None:
        path.append((current.x, current.y))
        current = current.parent
    return path[::-1]


def a_star_algorithm(start, goal, grid):
    open_list = []
    heapq.heappush(open_list, start)
    closed_list = set()

    while open_list:
        current = heapq.heappop(open_list)

        if (current.x, current.y) == (goal.x, goal.y):
            return find_path(start, current)

        closed_list.add((current.x, current.y))

        for dx in [-1, 0, 1]:
            for dy in [-1, 0, 1]:
                if dx == 0 and dy == 0:
                    continue
                if dx != 0 and dy != 0:
                    continue

                newX, newY = current.x + dx, current.y + dy

                if newX < 0 or newX >= len(grid) or newY < 0 or newY >= len(grid[0]) or grid[newX][newY] == 1:
                    continue

                neighbor = Node(newX, newY)
                neighbor.G = current.G + 1
                neighbor.H = calculate_heuristic(neighbor, goal)
                neighbor.parent = current

                if (newX, newY) not in closed_list:
                    heapq.heappush(open_list, neighbor)

    return None


if __name__ == "__main__":
    grid = [
        [0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1],
        [1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1],
        [1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1],
        [1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1],
        [1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1],
        [1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1],
        [1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1],
        [1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1],
        [1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
        [1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1],
        [1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1],
        [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0]
    ]

    start_node = Node(0, 0)
    goal_node = Node(15, 15)

    path = a_star_algorithm(start_node, goal_node, grid)

    if path:
        print("Path found:", path)
    else:
        print("No path found")

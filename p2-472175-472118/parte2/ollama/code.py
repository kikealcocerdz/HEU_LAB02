from collections import deque
import heapq
import math


def a_star(start, goal, graph):
    # Initialize the priority queues and hash table
    pq = []
    estimated_costs = {}

    # Add the starting node to the priority queue
    heapq.heappush(pq, (0, start))

    while pq:
        # Pop the node with the lowest estimated total cost from the priority queue
        current, estimated_cost = heapq.heappop(pq)

        # If the popped node is the goal node, return the path found
        if current == goal:
            return get_path(start, current)

        # Mark the current node as explored and add all of its neighbors to the priority queue
        try:
            graph[current].explored = True
        except KeyError:
            print("Node at position", current, "does not exist in the graph.")

        for neighbor in graph[current].neighbors:
            if not graph[neighbor].explored:
                heapq.heappush(pq, (estimated_cost + math.sqrt(abs(graph[current].position -
                                                                   graph[neighbor].position)), neighbor))

        # Update the estimated costs of each node in the priority queue
        for i in range(len(pq)):
            current, estimated_cost = pq[i]
            estimated_costs[current] = estimated_cost

    return "No path found"


def get_path(start, goal):
    # Initialize the path with the starting node
    path = [start]

    # Follow the parent pointers back to the start node and construct the path
    current = goal
    while current != start:
        current = graph[current].parent
        path.append(current)

    return path

graph = {
    "start": {"position": (0, 0), "explored": False},
    "goal": {"position": (10, 10), "explored": False},
    "node_1": {"position": (5, 5), "parent": "start", "explored": False},
    "node_2": {"position": (8, 8), "parent": "start", "explored": False},
    "node_3": {"position": (10, 5), "parent": "start", "explored": False}
}
start = "start"
goal = "goal"
path = a_star(start, goal, graph)

path = [start] + get_path(start, goal)
print(path)
# Output: ["start", "node_1", "node_2", "node_3", "goal"]

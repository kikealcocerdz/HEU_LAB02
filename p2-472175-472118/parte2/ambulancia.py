import pandas as pd

class Node():
    """
    Nodo para hacer el algoritmo A*
    """
    def __init__(self, parent=None, position=None):
        self.parent = parent
        self.position = position

        self.g = 0
        self.h = 0
        self.f = 0

    def __eq__(self, other):
        return self.position == other.position


def astar(map, starting_point, ending_point):
    """
    Returns a list of tuples as a path from the given start to the given end in the given maze
    :param map:
    :param starting_point:
    :param ending_point:
    :return:
    """
    # Inicializamos los nodos de inicio y final
    start_node = Node(None, starting_point)
    start_node.g = start_node.h = start_node.f = 0
    end_node = Node(None, ending_point)
    end_node.g = end_node.h = end_node.f = 0

    # Inicializamos las listas abierta y cerrada
    open_list = []
    closed_list = []

    open_list.append(start_node)
    while len(open_list) > 0:
        current_node = open_list[0]
        current_index = 0
        for index, item in enumerate(open_list):
            if item.f < current_node.f:
                current_node = item
                current_index = index

        open_list.pop(current_index)
        closed_list.append(current_node)

        if current_node == end_node:
            path = []
            current = current_node
            while current is not None:
                path.append(current.position)
                current = current.parent
            return path[::-1] # Return reversed path

        children = []
        for new_position in [(0, -1), (0, 1), (-1, 0), (1, 0)]: # Movimientos posibles
            # Obtenemos la posicion del nodo
            node_position = (current_node.position[0] + new_position[0], current_node.position[1] + new_position[1])

            # Nos aseguramos de que no se salga del mapa
            if node_position[0] > (len(map) - 1) or node_position[0] < 0 or node_position[1] > (len(map[len(map)-1]) -1) or node_position[1] < 0:
                continue

            # Nos aseguramos de que no se pueda pasar por un obstaculo
            if map[node_position[0]][node_position[1]] != 0:
                continue

            # Creamos un nuevo nodo
            new_node = Node(current_node, node_position)

            # Anadimos el nuevo nodo a la lista de hijos
            children.append(new_node)

        # Loop through children
        for child in children:
            # El hijo esta en la lista cerrada
            for closed_child in closed_list:
                if child == closed_child:
                    continue

            # Creamos los valores de f, g y h
            child.g = current_node.g + 1
            child.h = ((child.position[0] - end_node.position[0]) ** 2) + ((child.position[1] - end_node.position[1]) ** 2)
            child.f = child.g + child.h

            # El hijo esta en la lista abierta
            for open_node in open_list:
                if child == open_node and child.g > open_node.g:
                    continue

            # Anadimos el hijo a la lista abierta
            open_list.append(child)

def main():
    maze = [[0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]

    start = (0, 0)
    end = (7, 6)

    path = astar(maze, start, end)
    print(path)

main()


data = pd.read_csv('mapa.csv', sep=';', header=None)

no_transitables = []
no_contagiosos = []
contagiosos = []
aten_contagiosos = []
aten_regulares = []
coste_2 = []
coste_1 = []
filas = len(data)
columnas = len(data[0])

for i in range(len(data)):
    for j in range(len(data[0])):
        if data[i][j] == 'P':
            starting_point = (i+1, j+1)
        elif data[i][j] == 'X':
            no_transitables.append((i+1, j+1))
        elif data[i][j] == 'C':
            contagiosos.append((i+1, j+1))
        elif data[i][j] == 'N':
            no_contagiosos.append((i+1, j+1))
        elif data[i][j] == '2':
            coste_2.append((i+1, j+1))
        elif data[i][j] == 'CC':
            aten_contagiosos.append((i+1, j+1))
        elif data[i][j] == 'CN':
            aten_regulares.append((i+1, j+1))
        else:
            coste_1.append((i+1, j+1))

print('Filas: ', filas, '\tColumnas: ', columnas)
print('Punto de partida: ', starting_point)
print('No transitables: ', no_transitables)
print('Contagiosos: ', contagiosos)
print('No contagiosos: ', no_contagiosos)
print('Atencion contagiosos: ', aten_contagiosos)
print('Atencion regulares: ', aten_regulares)
print('Coste 2: ', coste_2)
print('Coste 1: ', coste_1)






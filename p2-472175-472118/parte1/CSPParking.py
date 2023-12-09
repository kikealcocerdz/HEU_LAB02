from constraint import *
from pprint import pprint
import pandas as pd
import time
import random
import sys
import os


def escribe_sol(solucion):
    parking = {}
    for i in range(1, filas+1):
        for j in range(1, columnas+1):
            parking[(i,j)] = "-"
    solution_cars = [[value[0], value[1], key] for key, value in solucion.items()]

    for car in solution_cars:
        row, col, car_id = car
        parking[(row, col)] = car_id
    
    resultado = ""
    comas_ocupado = ","
    comas_libre = ","
    try:
        if sys.argv[3] == "bonito":
            comas_ocupado = "\t,\t"
            comas_libre = "\t\t\t,\t"
    except IndexError:
        pass
    for i in range(1, filas+1):
        for j in range(1, columnas+1):
            if len(parking[(i,j)]) > 4: # Si hay coche
                addition = f"\"{parking[(i,j)]}\"{comas_ocupado}"
            else:
                addition = f"\"{parking[(i,j)]}\"{comas_libre}"
            resultado += addition
        resultado += "\n"
    return resultado




ruta_parking = sys.argv[1]

with open(ruta_parking, 'r') as archivo:
    lineas = archivo.readlines()
    # quiero no guardarme los /n
    lineas = [linea.strip() for linea in lineas]
    # Toma los coches que hay que meter en el parking
    coches = lineas[2:]

    # Número de coches para meter en el parking
    num_coches = len(coches) 
    matriz = lineas[0].split("x")
    global filas, columnas
    filas = int(matriz[0]) # Número de filas del parking
    columnas = int(matriz[1]) # Número de columnas del parking

    cargadores = lineas[1].replace("PE:", "").replace(")", ")p").split("p")[:-1]
    cargadores = [(int(cargador[1]), int(cargador[-2])) for cargador in cargadores]
    


# Un parking tiene sus aparcamientos numerados por filas y columnas, la columna 7 es la salida del garaje
# Los vehiculos que aparcan en esto aparcamientos pueden ser de dos tipos, TSU y TNU
problem = Problem()

# Crea una matriz de todas las plazas del parking disponibles, como elementos vacios de una lista de listas
plazas = [(i, j) for j in range(1, columnas + 1) for i in range(1, filas + 1)]

# Crea el dominio, asigna a cada coche todas las plazas posibles.
problem.addVariables(coches, plazas)


# Crea las restricciones
# La primera restricción es el comportamiento por defecto en python-constraints

# 2. Cada coche tiene que tener una plaza distinta
problem.addConstraint(AllDifferentConstraint())



# 3. Cada coche con congelador, tiene que tener una conexión eléctrica en su plaza
# Toma solo los coches que tienen un congelador e itera por la lista
for coche in [coche for coche in coches if coche[-1] == "C"]:
    problem.addConstraint(lambda plaza: plaza in cargadores, [coche])


# 4. Vehículos de tipo TSU no pueden tener delante un vehículo de tipo TNU. 
# Vamos, que la segunda coordenada de la plaza del TSU tiene que ser menor que la del TNU
for i in range(0, len(coches)):
    for j in range(0, len(coches)):
        if "S" in coches[i] and "N" in coches[j]:
            problem.addConstraint(lambda plaza1, plaza2:
                                   int(plaza1[1]) > int(plaza2[1]) if (plaza1[0]==plaza2[0]) else True,
                                     [coches[i], coches[j]])


# 5. Por cuestiones de maniobrabilidad dentro del parking todo vehículo debe tener libre una plaza a izquierda
# o derecha (mirando en direcci´on a la salida). Por ejemplo, si un veh´ıculo ocupa la plaza 3.3 no podrá tener
# aparcado un vehículo en la 2.3 y otro en la 4.3, al menos una de esas dos plazas deberá quedar libre.

def comprueba(plaza1: str, plaza2: str, plaza3: str) -> bool:
    if plaza1[0] == 1 or plaza1[0] == filas:
        if abs(int(plaza1[0]) - int(plaza2[0])) == 1 or abs(int(plaza1[0]) - int(plaza3[0])) == 1:
            return False
        
    if abs(int(plaza1[0]) - int(plaza2[0])) == 1 and abs(int(plaza1[0]) - int(plaza3[0])) == 1:
        if (plaza1[1] == plaza2[1]) and (plaza1[1] == plaza3[1]) and (plaza2[1] == plaza3[1]):
            return False

    return True
    
        
    """if plaza1[1] != plaza2[1] or plaza1[1] != plaza3[1]:
        return True
    
    if int(plaza1[0]) == 1 or int(plaza1[0]) == filas:
        if abs(int(plaza2[0]) - int(plaza1[0])) == 1 or abs(int(plaza3[0]) - int(plaza1[0])) == 1:
            return False

    
    
    if abs(int(plaza1[0]) - int(plaza2[0])) == 1 and abs(int(plaza1[0]) - int(plaza3[0])) == 1:
        return False

    return True

    if plaza1[1] != plaza2[1] :
        return True
    if int(plaza1[0]) == 1 or int(plaza1[0]) == filas:
        if abs(int(plaza2[0]) - int(plaza1[0])) == 1:
            return False
        if abs(int(plaza3[0]) - int(plaza1[0])) == 1:
            return False

    if abs(int(plaza1[0]) - int(plaza2[0])) == 1 and abs(int(plaza1[0]) - int(plaza3[0])) == 1:
        return False
    
    return True"""



for i in range(0, len(coches)):
    for j in range(0, len(coches)):
        for k in range(0, len(coches)):
            if i != j and i != k and j != k:
                problem.addConstraint(comprueba, [coches[i], coches[j], coches[k]])


# Salida del programa
solutions = problem.getSolutions()
solutions_dictionary = [{v: k for k, v in solutions.items()} for solutions in solutions]


# Generar el nombre del archivo de salida
nombre_archivo = str(ruta_parking)
nombre_salida = os.path.splitext(nombre_archivo)[0] + ".csv"

# Escribir los datos en el archivo de salida
with open(nombre_salida, "w") as archivo:
    archivo.write("N. Sol:" + solutions.__len__().__str__() + "\n")
    # Escribir las soluciones encontradas
    try:
        if sys.argv[2] == "todas":
            solutions_indexes = [i for i in range(solutions.__len__())]
    except IndexError:
        solutions_indexes = [random.randint(0, solutions.__len__()) for _ in range(15)]
    for solution in solutions_indexes:
        archivo.write(escribe_sol(solutions[solution]))
        archivo.write("\n")
        
    
print("Soluciones: ", solutions.__len__(), "\n")

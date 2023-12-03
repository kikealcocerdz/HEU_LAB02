from constraint import *
from pprint import pprint
import time
import sys
import os

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
    filas = int(matriz[0]) # Número de filas del parking
    columnas = int(matriz[1]) # Número de columnas del parking

    cargadores = lineas[1][3:].split(") (")
    cargadores[0] = cargadores[0][1:]
    cargadores[-1] = cargadores[-1][:-1]
    cargadores = [cargador.replace(",", "") for cargador in cargadores] # Posiciones de los cargadores



# Un parking tiene sus aparcamientos numerados por filas y columnas, la columna 7 es la salida del garaje
# Los vehiculos que aparcan en esto aparcamientos pueden ser de dos tipos, TSU y TNU
problem = Problem()

# Crea una matriz de todas las plazas del parking disponibles, como elementos vacios de una lista de listas
plazas = [[str(i)+str(j) for j in range(1, columnas + 1)] for i in range(1, filas + 1)]
plazas = [plaza for linea in plazas for plaza in linea]

# Crea el dominio
print("Coches: ", coches, "\n")
print("Plazas: ", plazas, "\n")
print("Cargadores: ", cargadores, "\n")

problem.addVariables(coches, plazas)


# Crea las restricciones
# La primera restricción es el comportamiento por defecto en python-constraints

# 2. Cada coche tiene que tener una plaza distinta
problem.addConstraint(AllDifferentConstraint(), coches)



# 3. Cada coche con congelador, tiene que tener una conexión eléctrica en su plaza
print("Coches con congelador: ", [coche for coche in coches if coche[-1] == "C"])
for coche in coches:
    if coche[-1] == "C":
        problem.addConstraint(lambda plaza: plaza in cargadores, [coche])


# 4. Vehículos de tipo TSU no pueden tener delante un vehículo de tipo TNU. 
# Vamos, que la segunda coordenada de la plaza del TSU tiene que ser menor que la del TNU
for i in range(0, len(coches)):
    for j in range(i + 1, len(coches)):
        if coches[i][3] == "S" and coches[j][3] == "N":
            problem.addConstraint(lambda plaza1, plaza2:
                                   int(plaza1[1]) > int(plaza2[1]) if (plaza1[0]==plaza2[0]) else True,
                                     [coches[i], coches[j]])


# 5. Por cuestiones de maniobrabilidad dentro del parking todo vehículo debe tener libre una plaza a izquierda
# o derecha (mirando en direcci´on a la salida). Por ejemplo, si un veh´ıculo ocupa la plaza 3.3 no podrá tener
# aparcado un vehículo en la 2.3 y otro en la 4.3, al menos una de esas dos plazas deber´a quedar libre.
for i in range(0, len(coches)):
    for j in range(i + 1, len(coches)):
        problem.addConstraint(lambda plaza1, plaza2:
                               abs(int(plaza1[0]) - int(plaza2[0])) > 1 if (plaza1[1]==plaza2[1]) else True,
                                 [coches[i], coches[j]])

a = time.time()
# Resuelve el problema
solutions = problem.getSolutions()
#pprint(solutions)
b = time.time()
print("Tiempo de ejecución: ", b - a, "\n")
print("Soluciones: ", problem.getSolutions().__len__(), "\n")
exit()


nombre_archivo = str(ruta_parking)

# Generar el nombre del archivo de salida
nombre_salida = os.path.splitext(nombre_archivo)[0] + ".csv"

# Escribir los datos en el archivo de salida
with open(nombre_salida, "w") as archivo:
    # Escribir el número de soluciones encontradas
    archivo.write("N. Sol:" + problem.getSolutions().__len__().__str__() + "\n")
    

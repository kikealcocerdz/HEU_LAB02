from constraint import *
import sys
import os

ruta_parking = sys.argv[1]

with open(ruta_parking, 'r') as archivo:
    lineas = archivo.readlines()
    # quiero no guardarme los /n
    lineas = [linea.strip() for linea in lineas]
    coches = len(lineas) - 2
    matriz = lineas[0].split("x")
    filas = int(matriz[0])
    columnas = int(matriz[1])

# Un parking tiene sus aparcamientos numerados por filas y columnas, la columna 7 es la salida del garaje
# Los vehiculos que aparcan en esto aparcamientos pueden ser de dos tipos, TSU y TNU
# En las plazas (1,1), (1,2), (2,1), (4,1), (5,1), (5,2) hay conexión eléctrica
problem = Problem()

letras = []
letra = "a"
for i in range(coches):
    # Código a ejecutar en cada iteración del bucle
    letra = chr(ord(letra) + 1)
    letras.append(letra)

for i in range(coches):
  problem.addVariable(letras[i], [range(1, filas), range(1,columnas)])
  coches -= 1


# Restricciones
# Todos los vehículos tienen asignada una sola plaza

nombre_archivo = str(ruta_parking)

# Generar el nombre del archivo de salida
nombre_salida = os.path.splitext(nombre_archivo)[0] + ".csv"

# Escribir los datos en el archivo de salida
with open(nombre_salida, "w") as archivo:
    # Escribir el número de soluciones encontradas
    archivo.write("N. Sol:" + problem.getSolutions().__len__().__str__() + "\n")
    

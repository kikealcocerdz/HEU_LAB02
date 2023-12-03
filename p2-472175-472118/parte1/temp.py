a = time.time()
# Resuelve el problema
problem.getSolutions()
b = time.time()
print("Tiempo de ejecución: ", b - a, "\n")
print("Soluciones: ", problem.getSolutions().__len__(), "\n")
exit()
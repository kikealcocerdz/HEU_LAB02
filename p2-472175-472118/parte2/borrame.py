with open('mapa.csv', 'r') as f:
    data = f.readlines()

for row in range(len(data)):
    lista = data[row].split(';')
    lista[-1] = lista[-1][0:-1]
    #print(lista)

exit(0)
from constraint import Problem, AllDifferentConstraint

def define_domain():
    return {'plaza': [(i, j) for i in range(1, 6) for j in range(1, 6)],
            'tipo_vehiculo': ['normal', 'congelador', 'TSU', 'TNU']}

def constraint_func(assignment):
    plaza = assignment['plaza']
    tipo_vehiculo = assignment['tipo_vehiculo']

    # Restricción 1: Todo vehículo debe tener asignada una plaza y solo una.
    if len(set(plaza)) != len(plaza):
        return False

    # Restricción 2: Dos vehículos distintos no pueden ocupar la misma plaza.
    if len(set(zip(tipo_vehiculo, plaza))) != len(tipo_vehiculo):
        return False

    # Restricción 3: Los vehículos con congelador solo pueden ocupar plazas con conexión eléctrica.
    for vehiculo, (i, j) in zip(tipo_vehiculo, plaza):
        if vehiculo == 'congelador' and (i, j) not in [(1, 2), (2, 2), (3, 2), (4, 2), (5, 2)]:
            return False

    # Restricción 4: Un TSU no puede tener aparcado por delante a ningún otro vehículo excepto si es TSU.
    for vehiculo, (i, j) in zip(tipo_vehiculo, plaza):
        if vehiculo == 'TSU':
            for k in range(j + 1, 6):
                if ('TSU', (i, k)) in zip(tipo_vehiculo, plaza) and ('TNU', (i, k)) in zip(tipo_vehiculo, plaza):
                    return False

    # Restricción 5: Por cuestiones de maniobrabilidad, cada vehículo debe tener libre una plaza a izquierda o derecha.
    for vehiculo, (i, j) in zip(tipo_vehiculo, plaza):
        if (i-1, j) in plaza or (i+1, j) in plaza:
            continue
        else:
            return False

    return True

def solve_parking_problem():
    problem = Problem()
    domain = define_domain()

    for variable, values in domain.items():
        problem.addVariable(variable, values)

    problem.addConstraint(AllDifferentConstraint(), ['plaza'])  # Garantiza que cada plaza sea única
    problem.addConstraint(constraint_func)

    solution = problem.getSolution()

    return solution

asignacion = solve_parking_problem()
print("Asignación de vehículos a plazas:", asignacion)


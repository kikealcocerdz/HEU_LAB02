echo -e "\nTest 1: 1x1 con 1 coche:"
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-1.txt bonito 
echo -e "Numero de soluciones esperado: 1\n\n"

echo -e "\nTest 2: 1x1 con 2 coches:"
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-2.txt bonito
echo -e "Numero de soluciones esperado: 0\n\n"

echo -e "\nTest 3: 1x2 con 1 TNU y 1 TSU:"
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-3.txt bonito
echo -e "Numero de soluciones esperado: 0\n\n"

echo -e "\nTest 4. 2x1 con 1 TNU CON congelador."
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-4.txt bonito
echo -e "Numero de soluciones esperado: 1\n\n"

echo -e "\nTest 5. 2x1 con 1 TNU SIN congelador"
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-5.txt bonito
echo -e "Numero de soluciones esperado: 2\n\n"

echo -e "\nTest 6. 3x1 con 2 TNU CON congelador"
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-6.txt bonito
echo -e "Numero de soluciones esperado: 2\n\n"

echo -e "\nTest 7. 3x2 con 1 TNU y 1 TSU CON congelador, cargadores (1,1) y (1, 2)"
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-7.txt bonito
echo -e "Numero de soluciones esperado: 0\n\n"

echo -e "\nTest 8. 3x3 con 2 TNU y 1 TSU CON congelador, cargadores (1,1) y (1, 2)"
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-8.txt bonito
echo -e "Numero de soluciones esperado: 0\n\n"

echo -e "\nTest 9. 3x3 con 3 coches electrificados, 3 cargadores"
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-9.txt
echo -e "Numero de soluciones esperado: 2\n\n"

echo -e "\nTest 10. 3x3 con 5 coches, 2 electrificados, 3 cargadores"
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-10.txt
echo -e "Numero de soluciones esperado: 84\n\n"


echo -e "\nTest 11. 3x4 con 5 coches, 2 electrificados, 3 cargadores"
python3 parte1/CSPParking.py parte1/test_calls/CSP-calls-test-11.txt
echo -e "Numero de soluciones esperado: 540\n\n"
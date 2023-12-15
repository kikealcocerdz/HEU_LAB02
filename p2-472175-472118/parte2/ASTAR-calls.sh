#!/bin/sh
cmake --version
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

cp ASTAR-tests/* build/
cd build
./main mapa-short.csv 1 > /dev/null
./main mapa-short.csv 2 > /dev/null
./main mapa-large.csv 1 > /dev/null
./main mapa-large.csv 2 > /dev/null
./main mapa-original.csv 1 > /dev/null
./main mapa-original.csv 2 > /dev/null





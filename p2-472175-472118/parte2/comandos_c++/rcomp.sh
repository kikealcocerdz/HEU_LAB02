#!/bin/sh
cmake --version
cmake -S .. -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
cd build
time ./main ../mapa.txt 1
time ./main ../mapa.txt 2

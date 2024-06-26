#!/bin/bash

# configures the repo
cmake -S . -B build -DCMAKE_BUILD_TYPE=Gprof

# builds the repo
cmake --build build

# generates the gmon.out file
./build/apps/app

# generates the analysis
gprof ./build/apps/app gmon.out > app-gprof.txt

# runs the binary and generates the corresponding graph
# -m: running the module
# -sw: strip function parameters & wrap function names
# -Gdpi: the resolution of the output file
# -Tpng: the output format
gprof ./build/apps/app gmon.out | python3 -m  gprof2dot | dot -Gdpi=250  -Tpng -o app-gprof-graph.png

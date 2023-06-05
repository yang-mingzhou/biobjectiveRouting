#!/bin/bash
module load gcc/11.3.0
# Compile CPP files
gcc -O3 -std=c99 -o testbhepv.o testbhepv.cpp bhepv.cpp bodPathRetrieval.c heap.c bod.c graph.c -lstdc++
gcc -O3 -std=c99 -o testbhepvHbor.o testbhepvHbor.cpp hborWithBhepv.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++
# MAP="COL"
# NPARTITIONS="8"

# echo "MAP=$MAP"
# echo "NPARTITIONS=$NPARTITIONS"

./testbhepv.o
./testbhepvHbor.o

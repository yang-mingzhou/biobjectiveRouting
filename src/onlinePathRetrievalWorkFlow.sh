#!/bin/bash
module load gcc/11.3.0
# Compile CPP files
gcc -O3 -std=c99 -o online.o onlineRouting.cpp hbor.cpp biobjectiveGraph.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++

MAP="COL"
NPARTITIONS="8"

echo "MAP=$MAP"
echo "NPARTITIONS=$NPARTITIONS"

./online.o ${MAP} ${NPARTITIONS}

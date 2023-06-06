#!/bin/bash
module load gcc/11.3.0
# Compile CPP files
gcc -O3 -std=c++11 -o kahip.o kahip.cpp  -lstdc++
gcc -O3 -std=c99 -o precomputation.o precomputation.cpp hbor.cpp biobjectiveGraph.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++
gcc -O3 -std=c++11 -o m.o m.cpp -lstdc++

MAP="test"
NPARTITIONS="3"

echo "MAP=$MAP"
echo "NPARTITIONS=$NPARTITIONS"

# Execute intermediate processing
./kahip.o ../Maps/${MAP}-road-d.txt ../b3hepv/${MAP}/kahip.graph

../../kahip/KaHIP/deploy/graphchecker ../b3hepv/${MAP}/kahip.graph 
../../kahip/KaHIP/deploy/kaffpa ../b3hepv/${MAP}/kahip.graph --output_filename=../b3hepv/${MAP}/kaffpaIndex.txt --k ${NPARTITIONS} --preconfiguration=strong

# Execute final processing
./precomputation.o ${MAP}

./m.o ${MAP}

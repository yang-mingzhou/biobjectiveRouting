#!/bin/bash
module load gcc/11.3.0
# Compile CPP files
gcc -O3 -std=c++11 -o kahip.o kahip.cpp  -lstdc++
# # -g -fsanitize=address
gcc -O3 -std=c99 -o b3hepvPrecomputation.o b3hepvPrecomputation.cpp b3hepv.cpp biobjectiveGraph.cpp bodPathRetrieval.c heap.c bod.c graph.c -lm -lstdc++
# gcc -O3 -std=c++11 -o encodeB3hepv.o m.cpp -lm -lstdc++
gcc -O3 -std=c++11 -o encodeB3hepv.o encodingB3hepv_b.cpp dijkstraForBoundaryEPV.cpp boundaryPath.cpp -lm -lstdc++
gcc -O3 -std=c99 -o b3hepvPathRetrieval.o b3hepvPathRetrieval.cpp hborWithB3hepv.cpp boundaryPath.cpp pathRetrieval.c heap.c boastar.c graph.c -lm -lstdc++


MAP="test"
NPARTITIONS="3"

echo "MAP=$MAP"
echo "NPARTITIONS=$NPARTITIONS"

# Execute intermediate processing
./kahip.o ../Maps/${MAP}-road-d.txt ../b3hepv/${MAP}/kahip.graph
../../kahip/KaHIP/deploy/graphchecker ../b3hepv/${MAP}/kahip.graph 
../../kahip/KaHIP/deploy/kaffpa ../b3hepv/${MAP}/kahip.graph --output_filename=../b3hepv/${MAP}/kaffpaIndex.txt --k ${NPARTITIONS} --preconfiguration=strong
./b3hepvPrecomputation.o ${MAP} ${NPARTITIONS}
./encodeB3hepv.o ${MAP}
# Execute final processing
./b3hepvPathRetrieval.o ${MAP} ${NPARTITIONS}

#!/bin/bash
module load gcc/11.3.0
# Compile CPP files
gcc -O3 -std=c++11 -o kahip.o kahip.cpp  -lstdc++
# # -g -fsanitize=address
gcc -O3 -std=c99 -o bhepvPrecomputation.o bhepvPrecomputation.cpp bhepv.cpp bodPathRetrieval.c heap.c bod.c graph.c -lm -lstdc++
gcc -O3 -std=c99 -o bhepvPathRetrieval.o bhepvPathRetrieval.cpp hborWithBhepv.cpp pathRetrieval.c heap.c boastar.c graph.c -lm -lstdc++
gcc -O3 -std=c99 -o namoadrPathRetrievalWorkFlow.o namoadrPathRetrievalWorkFlow.cpp bhepvForNamoadr.cpp namoadrPathRetrieval.c heap.c namoadr.c graph.c -lm -lstdc++

MAP="test"
NPARTITIONS="3"

echo "MAP=$MAP"
echo "NPARTITIONS=$NPARTITIONS"

# Execute intermediate processing
./kahip.o ../Maps/${MAP}-road-d.txt ../bhepv/${MAP}/kahip.graph
../../kahip/KaHIP/deploy/graphchecker ../bhepv/${MAP}/kahip.graph 
../../kahip/KaHIP/deploy/kaffpa ../bhepv/${MAP}/kahip.graph --output_filename=../bhepv/${MAP}/kaffpaIndex.txt --k ${NPARTITIONS} --preconfiguration=strong
./bhepvPrecomputation.o ${MAP} ${NPARTITIONS}

# Execute final processing
./bhepvPathRetrieval.o ${MAP} ${NPARTITIONS}

./namoadrPathRetrievalWorkFlow.o ${MAP} ${NPARTITIONS}
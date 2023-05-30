#!/bin/bash
module load gcc/11.3.0
# Compile CPP files
gcc -O3 -std=c++11 -o kahip.o kahip.cpp  -lstdc++
gcc -O3 -std=c++11 -o precomputation.o precomputation.cpp -lstdc++

# Execute intermediate processing
./kahip.o ../Maps/COL-road-d.txt ../b3hepv/col/kahipCOL.graph

../../kahip/KaHIP/deploy/graphchecker ../b3hepv/col/kahipCOL.graph 
../../kahip/KaHIP/deploy/kaffpa ../b3hepv/col/kahipCOL.graph --output_filename=../b3hepv/col/kaffpaIndex.txt --k 100 --preconfiguration=strong

# Execute final processing
./precomputation.o
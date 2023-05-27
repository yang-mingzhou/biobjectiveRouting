#!/bin/bash

#SBATCH --job-name=dfs ##job name
#SBATCH -o output/dfs_%j.output  ## output file name
#SBATCH -e output/dfs_%j.error ## error log
#SBATCH --nodes=1
#SBATCH --ntasks=128
#SBATCH --time=3:00:00
#SBATCH --tmp=3g
#SBATCH --mem=128g
#SBATCH --mail-type=ALL
#SBATCH --mail-user=yang7492@umn.edu
#SBATCH -p amdsmall                                             

gcc -O3 -std=c++11 -o dfs map_of_map_no_print.cpp -lstdc++
./dfs

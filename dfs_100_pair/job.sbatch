#!/bin/bash

#SBATCH --job-name=dfs ##job name
#SBATCH -o output/dfs_%j.output  ## output file name
#SBATCH -e output/dfs_%j.error ## error log
#SBATCH --nodes=1
#SBATCH --ntasks=128
#SBATCH --time=10:00:00
#SBATCH --tmp=5g
#SBATCH --mem=128g
#SBATCH --mail-type=ALL
#SBATCH --mail-user=yang7492@umn.edu
#SBATCH -p amdsmall                                             

gcc -std=c++11 -o dfs fixed.cpp -lstdc++
./dfs
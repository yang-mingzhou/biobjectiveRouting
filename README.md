# A Hierarchical Algorithm for High Throughput Bi-Objective Routing: A Summary of Results

## Abstract:

Given an origin, a destination, and a network graph in which each
directed edge is associated with a pair of non-negative costs, the bi-
objective routing problem aims to find the Pareto-optimal path set
between the origin and the destination. This problem has societal
applications, such as addressing electric vehicle range anxiety by
minimizing travel time and vehicle energy consumption in location-
based services. The problem is challenging due to the tension be-
tween the necessity for high throughput (i.e. the ability to process
a large number of queries quickly) and the extensive number of
Pareto-optimal paths, rendering all-pair precomputation methods
infeasible. Prior work on bi-objective routing has mostly relied on
purely compute-on-demand approaches and is not efficient enough
for high throughput routing queries, or can only generate subset
approximations of the Pareto-optimal set. To address these limi-
tations, we propose a Hierarchical Bi-objective Routing (HBOR)
algorithm, which contains a Bi-objective Hierarchical Encoded Path
View (BHEPV) model that strikes a balance between the compute-
on-demand approaches and precomputation approaches. We also
propose a Bounds-based BHEPV (B3HEPV) to refine HBOR. Our
theoretical evaluation proves the Pareto-optimality of the path sets
retrieved from the proposed methods, and the computational ex-
periments using San Francisco Bay Area road network data show
that the proposed methods can run an order of magnitude faster
than state-of-the-art methods in terms of online runtime.

## [Requirements](https://github.com/yang-mingzhou/biobjectiveRouting):
```
GCC 11
C99
```
## Example Execution: 
```
## Directories for saving encoded path view of the "test" network
mkdir -p bhepv/test
mkdir -p b3hepv/test
cd src
## HBOR for the "test" network with 2 partitions
make run_B3HBOR MAP=test NPARTITIONS=2
## HBOR-B for the "test" network with 3 partitions
make run_B3HBOR MAP=test NPARTITIONS=3
## clean object files
make clean
```

## Files:
0. [Makefile](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/makefile): Makefile for the both HBOR and HBOR-B
1. [bhepvWorkflow.sh](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepvWorkflow.sh): Workflow script for HBOR
2. [b3hepvWorkflow.sh](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/b3hepvWorkflow.sh): Workflow script for HBOR-B
3. [bhepv.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepv.cpp): 
4. [bhepvPathRetrieval.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepvPathRetrieval.cpp):  
5. [bhepvPrecomputation.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepvPrecomputation.cpp):  
6. [biobjectiveGraph.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/biobjectiveGraph.cpp):  
7. [biobjectiveGraph.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/biobjectiveGraph.h):  
8. [dfs_class_v2.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/dfs_class_v2.h): DFS specifying only source node
9. [fragmentPathRetrieval.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/fragmentPathRetrieval.cpp):  
10. [hbor.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hbor.cpp):  
11. [hbor.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hbor.h):  
12. [hborWithBhepv.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hborWithBhepv.cpp):  
13. [hborWithBhepv.hpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hborWithBhepv.hpp):  
14. [json.hpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/json.hpp):  
15. [kahip.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/kahip.cpp):  
16. [m.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/m.cpp): main file to run DFS 
17. [onlineRouting.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/onlineRouting.cpp):  
18. [pairs.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/pairs.cpp): Generating queries; we use 12 as a random seed in the project.
19. [precomputation.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/precomputation.cpp):  

### Test cases:

20. [test.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/test.cpp):  

### Following files are built upon code repository [BOAstar](https://github.com/jorgebaier/BOAstar/)   after fixing some memory leak issues:

21. [boastar.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/boastar.c):  
22. [boastar.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/boastar.h):  
23. [bod.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bod.c):  
24. [bod.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bod.h):  
25. [bodPathRetrieval.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bodPathRetrieval.c):  
26. [bodPathRetrieval.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bodPathRetrieval.h):  
27. [pathRetrieval.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/pathRetrieval.c):  
28. [pathRetrieval.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/pathRetrieval.h):  
29. [main_boa.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/main_boa.c):  
30. [main_bod.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/main_bod.c):  
31. [include.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/include.h):  
32. [node_namoa.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/node_namoa.h):  
33. [main_namoadr.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/main_namoadr.c):  
34. [namoadr.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/namoadr.c):  
35. [namoadr.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/namoadr.h):  
36. [node.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/node.h):  
37. [heap.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/heap.c):  
38. [heap.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/heap.h):  
39. [graph.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/graph.c):  
40. [graph.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/graph.h):  
41. [graph_data.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/graph_data.h):  


## File Folders:

1. Folder [Maps](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/Maps)
2. Folder [Queries](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/Queries)
3. Folder [src](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/src)

## Acknowledgements

This repository builds upon the following papers and code repositories:

### Paper
[Simple and efficient bi-objective search algo-
rithms via fast dominance checks](https://www.sciencedirect.com/science/article/pii/S0004370222001473)  
Carlos Hernández, William Yeoh, Jorge A Baier, Han Zhang, Luis Suazo, Sven
Koenig, and Oren Salzman. 2023. Simple and efficient bi-objective search algo-
rithms via fast dominance checks. Artificial Intelligence 314 (2023), 103807

[Think Locally, Act Globally: Highly
Balanced Graph Partitioning](https://link.springer.com/chapter/10.1007/978-3-642-38527-8_16)  
Peter Sanders and Christian Schulz. 2013. Think Locally, Act Globally: Highly
Balanced Graph Partitioning. In Experimental Algorithms, 12th International
Symposium, SEA 2013, Rome, Italy, June 5-7, 2013. Proceedings, Vol. 7933. Springer,
164–175.

### Code Repository
Repository Name: [BOAstar](https://github.com/jorgebaier/BOAstar/)  
Author(s)/Owner: jorgebaier 
Description: A C implementation of Bi-Objective A* (BOA*), New Approach to Multi-Objective A* with dimensionality reduction (NAMOA*dr), and Bi-Objective Dijkstra (BOD). 

Repository Name: [KaHIP](https://github.com/KaHIP/KaHIP)  
Author(s)/Owner: Karlsruhe High Quality Graph Partitioning 
Description: The graph partitioning framework KaHIP -- Karlsruhe High Quality Partitioning.

Change Log
-----

### 13th June. 2023
Version 1.0: Initial Commit.

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
## HBOR for the "test" network
mkdir -p bhepv/test
cd src
chmod +x bhepvWorkflow.sh
./bhepvWorkflow.sh
```

## Files:
1. [bhepvWorkflow.sh](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepvWorkflow.sh): Workflow script for HBOR
2. [b3hepvWorkflow.sh](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/b3hepvWorkflow.sh): Workflow script for HBOR-B
3. [bhepv.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepv.cpp): (Need to update)
4. [bhepvPathRetrieval.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepvPathRetrieval.cpp): (Need to update)
5. [bhepvPrecomputation.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepvPrecomputation.cpp): (Need to update)
6. [biobjectiveGraph.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/biobjectiveGraph.cpp): (Need to update)
7. [biobjectiveGraph.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/biobjectiveGraph.h): (Need to update)
8. [dfs_class_v2.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/dfs_class_v2.h): DFS specifying only source node
9. [fragmentPathRetrieval.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/fragmentPathRetrieval.cpp): (Need to update)
10. [hbor.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hbor.cpp): (Need to update)
11. [hbor.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hbor.h): (Need to update)
12. [hborWithBhepv.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hborWithBhepv.cpp): (Need to update)
13. [hborWithBhepv.hpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hborWithBhepv.hpp): (Need to update)
14. [json.hpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/json.hpp): (Need to update)
15. [kahip.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/kahip.cpp): (Need to update)
16. [m.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/m.cpp): main file to run DFS 
17. [onlineRouting.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/onlineRouting.cpp): (Need to update)
18. [pairs.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/pairs.cpp): Generating queries; we use 12 as a random seed in the project.
19. [precomputation.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/precomputation.cpp): (Need to update)

### Test cases:

20. [test.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/test.cpp): (Need to update)

### Following files are built upon code repository [BOAstar](https://github.com/jorgebaier/BOAstar/)   after fixing some memory leak issues:

21. [boastar.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/boastar.c): (Need to update)
22. [boastar.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/boastar.h): (Need to update)
23. [bod.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bod.c): (Need to update)
24. [bod.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bod.h): (Need to update)
25. [bodPathRetrieval.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bodPathRetrieval.c): (Need to update)
26. [bodPathRetrieval.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bodPathRetrieval.h): (Need to update)
27. [pathRetrieval.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/pathRetrieval.c): (Need to update)
28. [pathRetrieval.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/pathRetrieval.h): (Need to update)
29. [main_boa.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/main_boa.c): (Need to update)
30. [main_bod.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/main_bod.c): (Need to update)
31. [include.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/include.h): (Need to update)
32. [node_namoa.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/node_namoa.h): (Need to update)
33. [main_namoadr.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/main_namoadr.c): (Need to update)
34. [namoadr.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/namoadr.c): (Need to update)
35. [namoadr.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/namoadr.h): (Need to update)
36. [node.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/node.h): (Need to update)
37. [heap.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/heap.c): (Need to update)
38. [heap.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/heap.h): (Need to update)
39. [graph.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/graph.c): (Need to update)
40. [graph.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/graph.h): (Need to update)
41. [graph_data.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/graph_data.h): (Need to update)


## File Folders:

1. Folder [Maps](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/Maps)
2. Folder [Queries](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/Queries)
3. Folder [src](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/src)

## Acknowledgements

This repository builds upon the following paper and code repository:

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

# Bi-objective Routing

## Abstract:

The eco-toll estimation problem quantifies the expected environmental cost (e.g., energy consumption, exhaust emissions) for a vehicle to travel along a path. This problem
is important for societal applications such as eco-routing, which aims to find paths with the lowest exhaust emission or energy need. The challenges of this problem are threefold: 
(1) the dependence of a vehicle’s eco-toll on its physical parameters; (2) the lack of access to data with eco-toll
information; and (3) the influence of contextual information (i.e. the connections of adjacent segments in the path) on the eco-toll of road segments. 
Prior work on eco-toll estimation has mostly relied on pure data-driven approaches and has high estimation errors given the limited training data. 
To address these limitations, we propose a novel Eco-toll estimation Physics-informed Neural Network framework (Eco-PiNN) using three novel ideas, namely, (1) a physics-informed decoder that integrates the physical laws of the vehicle engine into the network, (2) an attention-based contextual information encoder, and (3) a physics-informed regularization to reduce overfitting. Experiments on real-world
heavy-duty truck data show that the proposed method can greatly improve the accuracy of eco-toll estimation compared with state-of-the-art methods

## [Requirements](https://github.com/yang-mingzhou/biobjectiveRouting):
```
bintrees==2.2.0
geopandas==0.10.2
networkx==2.6.3
numpy==1.21.3
osmnx==0.16.1
pandas==1.2.4
plotly==5.3.1
psycopg2==2.9.1
Shapely==1.8.0
torch==1.11.0
torch_geometric==2.0.4
torchvision==0.12.0
tqdm==4.59.0
```
## Files:
1. [bhepv.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepv.cpp): (Need to update)
2. [bhepv.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepv.h): training and testing of EcoPiNN.
3. [bhepv.hpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepv.hpp): (Need to update)
4. [bhepvPathRetrieval.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepvPathRetrieval.cpp): (Need to update)
5. [bhepvPrecomputation.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepvPrecomputation.cpp): (Need to update)
6. [bhepvWorkflow.sh](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bhepvWorkflow.sh): (Need to update)
7. [biobjectiveGraph.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/biobjectiveGraph.cpp): (Need to update)
8. [biobjectiveGraph.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/biobjectiveGraph.h): (Need to update)
9. [boastar.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/boastar.c): (Need to update)
10. [boastar.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/boastar.h): (Need to update)
11. [bod.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bod.c): (Need to update)
12. [bod.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bod.h): (Need to update)
13. [bodPathRetrieval.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bodPathRetrieval.c): (Need to update)
14. [bodPathRetrieval.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/bodPathRetrieval.h): (Need to update)
15. [dfs_class.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/dfs_class.h): DFS for specific source and destination node
16. [dfs_class_v2.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/dfs_class_v2.h): DFS specifying only source node
17. [fragmentPathRetrieval.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/fragmentPathRetrieval.cpp): (Need to update)
18. [graph.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/graph.c): (Need to update)
19. [graph.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/graph.h): (Need to update)
20. [graph_data.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/graph_data.h): (Need to update)
21. [hbor.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hbor.cpp): (Need to update)
22. [hbor.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hbor.h): (Need to update)
23. [hborWithBhepv.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hborWithBhepv.cpp): (Need to update)
24. [hborWithBhepv.hpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/hborWithBhepv.hpp): (Need to update)
25. [heap.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/heap.c): (Need to update)
26. [heap.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/heap.h): (Need to update)
27. [include.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/include.h): (Need to update)
28. [json.hpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/json.hpp): (Need to update)
29. [kahip.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/kahip.cpp): (Need to update)
30. [m.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/m.cpp): main file to run DFS 
31. [main_boa.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/main_boa.c): (Need to update)
32. [main_bod.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/main_bod.c): (Need to update)
33. [main_namoadr.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/main_namoadr.c): (Need to update)
34. [namoadr.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/namoadr.c): (Need to update)
35. [namoadr.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/namoadr.h): (Need to update)
36. [node.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/node.h): (Need to update)
37. [node_namoa.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/node_namoa.h): (Need to update)
38. [onePairQuery.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/onePairQuery.cpp): (Need to update)
39. [onlinePathRetrievalWorkFlow.sh](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/onlinePathRetrievalWorkFlow.sh): (Need to update)
40. [onlineRouting.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/onlineRouting.cpp): (Need to update)
41. [pairs.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/pairs.cpp): (Need to update)
42. [pathRetrieval.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/pathRetrieval.c): (Need to update)
43. [pathRetrieval.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/pathRetrieval.h): (Need to update)
44. [precomputation.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/precomputation.cpp): (Need to update)
45. [precomputationWorkFlow.sh](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/precomputationWorkFlow.sh): (Need to update)
46. [syntheticGraph.sh](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/syntheticGraph.sh): (Need to update)
47. [test.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/test.cpp): (Need to update)
48. [testVector.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/testVector.c): (Need to update)
49. [testbhepv.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/testbhepv.cpp): (Need to update)
5. [testbhepv.sh](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/testbhepv.sh): (Need to update)
5. [testbhepvHbor.cpp](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/testbhepvHbor.cpp): (Need to update)
5. [vector.c](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/vector.c): (Need to update)
5. [vector.h](https://github.com/yang-mingzhou/biobjectiveRouting/blob/main/src/vector.h): (Need to update)





## File Folders:

1. Folder [Maps](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/Maps): (need to update)
2. Folder [Queries](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/Queries) contains the pretrained NODE2VEC model(detailed in Sec 3.1 of the paper).
3. Folder [b3hepv](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/b3hepv): (need to update)
4. Folder [dfs_100_pair](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/dfs_100_pair): (need to update)
5. Folder [src](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/src): (need to update)
6. Folder [src_cpp](https://github.com/yang-mingzhou/biobjectiveRouting/tree/main/src_cpp): (need to update)

   
   
## Datasets
We are not allowed to publish the on-board diagnostics (OBD) dataset described in the paper. 
In future works, we plan to generate some synthetic vehicle travel datasets using vehicle powertrain simulators 
(e.g., [FastSim](https://www.nrel.gov/transportation/fastsim.html#:~:text=The%20Future%20Automotive%20Systems%20Technology,%2C%20cost%2C%20and%20battery%20life.)).
We will publish the code for generating the synthetic datasets and the corresponding datasets in the future.

Change Log
-----

### 6th Oct. 2022
Version 1.0: Initial Commit.
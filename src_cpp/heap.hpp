#ifndef HEAP_HPP
#define HEAP_HPP

#include "include.hpp"
#include <vector>

class Heap_dij {
private:
    std::vector<gnode*> heap_dij;
    int heapsize_dij;
    unsigned long long stat_percolations;

    void percolatedown_dij(int hole, gnode* tmp);
    void percolateup_dij(int hole, gnode* tmp);
    void percolateupordown_dij(int hole, gnode* tmp);

public:
    Heap_dij();
    void insertheap_dij(gnode* thiscell);
    void deleteheap_dij(gnode* thiscell);
    gnode* topheap_dij();
    void emptyheap_dij();
    gnode* popheap_dij();
    int sizeheap_dij();
    gnode* posheap_dij(int i);
};

class Heap {
private:
    std::vector<snode*> heap;
    int heapsize;
    unsigned long long stat_percolations;

    void percolatedown(int hole, snode* tmp);
    void percolateup(int hole, snode* tmp);
    void percolateupordown(int hole, snode* tmp);

public:
    Heap();
    void insertheap(snode* thiscell);
    void deleteheap(snode* thiscell);
    snode* topheap();
    void emptyheap();
    snode* popheap();
    int sizeheap();
    long int opensize();
    snode* posheap(int i);
};

#endif

#ifndef MAZEH
#define MAZEH

#include "include.hpp"

class gnode // stores info needed for each graph node
{
public:
    long long int id;
    unsigned h1;
    unsigned h2;
    unsigned long long int key;
    unsigned gmin;
    unsigned long heapindex;
    class snode *gopfirst;
    class snode *goplast;

    gnode() : id(0), h1(0), h2(0), key(0), gmin(0), heapindex(0), gopfirst(nullptr), goplast(nullptr) {}
};


class snode // BOA*'s search nodes
{
public:
    int state;
    unsigned g1;
    unsigned g2;
    double key;
    unsigned long heapindex;
    class snode *searchtree;
    class snode *gopnext;
    class snode *gopprev;

    snode() : state(0), g1(0), g2(0), key(0.0), heapindex(0), searchtree(nullptr), gopnext(nullptr), gopprev(nullptr) {}
};

#endif

#include "heap.hpp"

// constructor for Heap_dij
Heap_dij::Heap_dij() {
    heap_dij.resize(HEAP_SIZE_DIJ, nullptr);
    heapsize_dij = 0;
    stat_percolations = 0;
}

void Heap_dij::percolatedown_dij(int hole, gnode* tmp) {
  int child;
  if (heapsize_dij != 0) {
    for (; 2 * hole + 1 < heapsize_dij; hole = child) {
      child = 2 * hole + 1;
      if (child != heapsize_dij - 1 && heap_dij[child + 1]->key < heap_dij[child]->key)
        ++child;
      if (heap_dij[child]->key < tmp->key) {
        heap_dij[hole] = heap_dij[child];
        heap_dij[hole]->heapindex = hole;
        ++stat_percolations;
      }
      else
        break;
    }
    heap_dij[hole] = tmp;
    heap_dij[hole]->heapindex = hole;
  }
}

void Heap_dij::percolateup_dij(int hole, gnode* tmp) {
  if (heapsize_dij != 0) {
    for (; hole > 0 && tmp->key < heap_dij[(hole - 1) / 2]->key; hole = (hole - 1) / 2) {
      heap_dij[hole] = heap_dij[(hole - 1) / 2];
      heap_dij[hole]->heapindex = hole;
      ++stat_percolations;
    }
    heap_dij[hole] = tmp;
    heap_dij[hole]->heapindex = hole;
  }
}

void Heap_dij::percolateupordown_dij(int hole, gnode* tmp) {
  if (heapsize_dij != 0) {
    if (hole > 0 && heap_dij[(hole - 1) / 2]->key > tmp->key)
      percolateup_dij(hole, tmp);
    else
      percolatedown_dij(hole, tmp);
  }
}

void Heap_dij::insertheap_dij(gnode* thiscell) {
  if (thiscell->heapindex == 0)
    percolateup_dij(heapsize_dij++, thiscell);
  else
    percolateupordown_dij(thiscell->heapindex, heap_dij[thiscell->heapindex]);
}

void Heap_dij::deleteheap_dij(gnode* thiscell) {
  if (thiscell->heapindex != 0) {
    percolateupordown_dij(thiscell->heapindex, heap_dij[--heapsize_dij]);
    thiscell->heapindex = 0;
  }
}

gnode* Heap_dij::topheap_dij() {
  if (heapsize_dij == 0)
    return nullptr;
  return heap_dij[0];
}

void Heap_dij::emptyheap_dij() {
  for (int i = 0; i < heapsize_dij; ++i)
    heap_dij[i]->heapindex = 0;
  heapsize_dij = 0;
}

gnode* Heap_dij::popheap_dij() {
  if (heapsize_dij == 0)
    return nullptr;
  gnode* thiscell = heap_dij[0];
  thiscell->heapindex = 0;
  percolatedown_dij(0, heap_dij[--heapsize_dij]);
  return thiscell;
}

int Heap_dij::sizeheap_dij() {
  return heapsize_dij;
}

gnode* Heap_dij::posheap_dij(int i) {
  return heap_dij[i];
}


// constructor for Heap
Heap::Heap() {
    heap.resize(HEAP_SIZE, nullptr);
    heapsize = 0;
    stat_percolations = 0;
}

void Heap::percolatedown(int hole, snode* tmp) {
  int child;
  if (heapsize != 0) {
    for (; 2 * hole + 1 < heapsize; hole = child) {
      child = 2 * hole + 1;
      if (child != heapsize - 1 && heap[child + 1]->key < heap[child]->key)
        ++child;
      if (heap[child]->key < tmp->key) {
        heap[hole] = heap[child];
        heap[hole]->heapindex = hole;
        ++stat_percolations;
      }
      else
        break;
    }
    heap[hole] = tmp;
    heap[hole]->heapindex = hole;
  }
}

void Heap::percolateup(int hole, snode* tmp) {
  if (heapsize != 0) {
    for (; hole > 0 && tmp->key < heap[(hole - 1) / 2]->key; hole = (hole - 1) / 2) {
      heap[hole] = heap[(hole - 1) / 2];
      heap[hole]->heapindex = hole;
      ++stat_percolations;
    }
    heap[hole] = tmp;
    heap[hole]->heapindex = hole;
  }
}

void Heap::percolateupordown(int hole, snode* tmp) {
  if (heapsize != 0) {
    if (hole > 0 && heap[(hole - 1) / 2]->key > tmp->key)
      percolateup(hole, tmp);
    else
      percolatedown(hole, tmp);
  }
}

void Heap::insertheap(snode* thiscell) {
  if (thiscell->heapindex == 0)
    percolateup(heapsize++, thiscell);
  else
    percolateupordown(thiscell->heapindex, heap[thiscell->heapindex]);
}

void Heap::deleteheap(snode* thiscell) {
  if (thiscell->heapindex != 0) {
    percolateupordown(thiscell->heapindex, heap[--heapsize]);
    thiscell->heapindex = 0;
  }
}

snode* Heap::topheap() {
  if (heapsize == 0)
    return nullptr;
  return heap[0];
}

void Heap::emptyheap() {
  for (int i = 0; i < heapsize; ++i)
    heap[i]->heapindex = 0;
  heapsize = 0;
}

snode* Heap::popheap() {
  if (heapsize == 0)
    return nullptr;
  snode* thiscell = heap[0];
  thiscell->heapindex = 0;
  percolatedown(0, heap[--heapsize]);
  return thiscell;
}

int Heap::sizeheap() {
  return heapsize;
}

long int Heap::opensize() {
  return heapsize_dij;
}

snode* Heap::posheap(int i) {
  return heap[i];
}

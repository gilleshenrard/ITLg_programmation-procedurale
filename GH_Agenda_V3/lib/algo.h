#ifndef ALGO_H_INCLUDED
#define ALGO_H_INCLUDED
#include <stdlib.h>

typedef struct {
    void* tab;
    int tabsize;
    int elementsize;
    int (*doCompare)(void*, void*);
    int (*doSwap)(void*, void*);
} t_algo_meta;

//Sorting algorithms
int bubbleSort(t_algo_meta*);
int quickSortPartitioning(t_algo_meta*, int, int);
int quickSort(t_algo_meta*, int, int);

//Research algorithms
int binarySearch(t_algo_meta*, void* toSearch);
int binarySearchFirst(t_algo_meta *meta, void* toSearch);

#endif // ALGO_H_INCLUDED

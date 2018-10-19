#ifndef ALGO_H_INCLUDED
#define ALGO_H_INCLUDED
#include <stdlib.h>

typedef struct {
    void* tab;
    int tabsize;
    int elementsize;
    int (*doCompare)(void*, void*);
    int (*doSwap)(void*, void*);
    int (*doAssign)(void*, void*);
} t_algo_meta;

//Sorting algorithms
int bubbleSort(t_algo_meta*);
int quickSortPartitioning(t_algo_meta*, int, int);
int quickSort(t_algo_meta*, int, int);

//Research algorithms
int binarySearch(t_algo_meta*, void* toSearch);
int binarySearchFirst(t_algo_meta *meta, void* toSearch);

//Dynamic lists
int appendUnsortedList(t_algo_meta* meta, void **first, void **last, void *toAdd,  void** (*next)(void*));

#endif // ALGO_H_INCLUDED

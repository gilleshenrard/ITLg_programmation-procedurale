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

typedef struct {
    t_algo_meta meta;
    int (*doAssign)(void*, void*);
    void** (*next)(void*);
} t_list_meta;

//Sorting algorithms
int bubbleSort(t_algo_meta*);
int quickSortPartitioning(t_algo_meta*, int, int);
int quickSort(t_algo_meta*, int, int);

//Research algorithms
int binarySearch(t_algo_meta*, void* toSearch);
int binarySearchFirst(t_algo_meta *meta, void* toSearch);

//Dynamic lists
int appendUnsortedList(t_list_meta* meta, void **first, void **last, void *toAdd);
int foreachList(t_list_meta* meta, void **first, void* parameter, int (*doAction)(void*, void*));

#endif // ALGO_H_INCLUDED

#ifndef ALGO_H_INCLUDED
#define ALGO_H_INCLUDED
#include <stdlib.h>

typedef struct {
    void* structure;
    int nbelements;
    int elementsize;
    int (*doCompare)(void*, void*);
    int (*doSwap)(void*, void*);
    int (*doCopy)(void*, void*);
    void** (*next)(void*);
} t_algo_meta;

//Sorting algorithms
void bubbleSort(t_algo_meta*);
void bubbleSortList(t_algo_meta*);
int quickSortPartitioning(t_algo_meta*, int, int);
void quickSort(t_algo_meta*, int, int);

//Research algorithms
int binarySearch(t_algo_meta*, void*);
int binarySearchFirst(t_algo_meta*, void*);

//Dynamic lists
int insertListTop(t_algo_meta*, void*);
int insertListSorted(t_algo_meta*,  void*);
void foreachList(t_algo_meta*, void*, int (*doAction)(void*, void*));

#endif // ALGO_H_INCLUDED

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
    int (*doCopy)(void*, void*);
    void** (*next)(void*);
} t_list_meta;

//Sorting algorithms
void bubbleSort(t_algo_meta*);
void bubbleSortList(t_list_meta*, void**);
int quickSortPartitioning(t_algo_meta*, int, int);
void quickSort(t_algo_meta*, int, int);

//Research algorithms
int binarySearch(t_algo_meta*, void* toSearch);
int binarySearchFirst(t_algo_meta *meta, void* toSearch);

//Dynamic lists
int insertListTop(t_list_meta* listmeta, void **first, void *toAdd);
int insertListSorted(t_list_meta *listmeta,  void** first, void* toAdd);
void foreachList(t_list_meta* meta, void **first, void* parameter, int (*doAction)(void*, void*));

#endif // ALGO_H_INCLUDED

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
    void** (*previous)(void*);
} t_algo_meta;

typedef enum {COPY, REPLACE} e_listtoarray;

//miscellaneous
int listToArray(t_algo_meta*, t_algo_meta*, e_listtoarray);

//Sorting algorithms
int bubbleSort(t_algo_meta*);
int bubbleSortList(t_algo_meta*);
int quickSortPartitioning(t_algo_meta*, int, int);
int quickSort(t_algo_meta*, int, int);

//Research algorithms
int binarySearch(t_algo_meta*, void*);
int binarySearchFirst(t_algo_meta*, void*);

//Dynamic lists
int insertListTop(t_algo_meta*, void*);
int popListTop(t_algo_meta*);
int insertListSorted(t_algo_meta*,  void*);
int foreachList(t_algo_meta*, void*, int (*doAction)(void*, void*));

//Arrays
int foreachArray(t_algo_meta*, void*, int (*doAction)(void*, void*));

#endif // ALGO_H_INCLUDED

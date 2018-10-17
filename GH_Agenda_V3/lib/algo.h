#ifndef ALGO_H_INCLUDED
#define ALGO_H_INCLUDED
#include <stdlib.h>

//Sorting algorithms
int bubbleSort(void* tab, int tabsize, int elementsize, int (*compare)(void*, void*), int (*swapping)(void*, void*));

//Research algorithms
int binarySearch(void* tab, int tabsize, int elementsize, int (*compare)(void*, void*), void* toSearch);

#endif // ALGO_H_INCLUDED

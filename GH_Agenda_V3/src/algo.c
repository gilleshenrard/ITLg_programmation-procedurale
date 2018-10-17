#include "../lib/algo.h"

/************************************************************/
/*  I : Array to sort                                       */
/*      Number of records in the array                      */
/*      Size of a record in memory                          */
/*      Comparison procedure (function pointer)             */
/*      Swapping procedure  (function pointer)              */
/*  P : Sorts the provided array using the Bubble Sort algo */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
int bubbleSort(void* tab, int tabsize, int elementsize, int (*compare)(void*, void*), int (*swapping)(void*, void*)){
    void *tmp = tab;

    for(int i=0 ; i<tabsize-1 ; i++){
        for(int j=0 ; j<tabsize-i-1 ; j++){
            //compare tab[i] and tab[i+1]
            if((*compare)(tmp+(elementsize*j), tmp+(elementsize*(j+1))) > 0)
                //swap tab[i] and tab[i+1]
                (*swapping)(tmp+(elementsize*j), tmp+(elementsize*(j+1)));
        }
    }

    return 0;
}

/************************************************************/
/*  I : Array to search in                                  */
/*      Number of records in the array                      */
/*      Size of a record in memory                          */
/*      Comparison procedure (function pointer)             */
/*      Element to search                                   */
/*  P : Searches the key using the Binary search algorithm  */
/*  O : = 0 -> Not found                                    */
/*     != 0 -> Index of the first occurence in the array    */
/************************************************************/
int binarySearch(void* tab, int tabsize, int elementsize, int (*compare)(void*, void*), void* toSearch){
    int i=0, j=tabsize-1, m=0;
    void *tmp = tab;

    //continue searching until the very first occurence in the array
    while(i<=j){
        m = (i+j)/2;
        if((*compare)(tmp+(elementsize*m), toSearch) < 0)
            i = m+1;
        else
            if((*compare)(tmp+(elementsize*m), toSearch) > 0)
                j = m-1;
            else
                return m;
    }

    return 0;
}

/************************************************************/
/*  I : Array to search in                                  */
/*      Number of records in the array                      */
/*      Size of a record in memory                          */
/*      Comparison procedure (function pointer)             */
/*      Element to search                                   */
/*  P : Finds the first occurence of the key using          */
/*          the Binary search algorithm                     */
/*  O : = 0 -> Not found                                    */
/*     != 0 -> Index of the first occurence in the array    */
/************************************************************/
int binarySearchFirst(void* tab, int tabsize, int elementsize, int (*compare)(void*, void*), void* toSearch){
    void* tmp = tab;

    //use the binary search to find an occurence of the element
    int i = binarySearch(tab, tabsize, elementsize, compare, toSearch);

    if(!i)
        return 0;

    //walk through all the occurences of the key until the first one
    while(i>=0 && (*compare)(tmp+(elementsize*i), toSearch) >= 0){
        i--;
    }
    return i+1;
}

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
/*  P : Searches in the array for the provided element      */
/*      WARNING : algorithm tweaked to search for the first */
/*          occurence in a sorted array (not applicable to  */
/*                                       every test case)   */
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
            j = m-1;
    }

    //check if found, and return result
    if((*compare)(tmp+(elementsize*m), toSearch) != 0)
        return 0;
    else
        return i;
}

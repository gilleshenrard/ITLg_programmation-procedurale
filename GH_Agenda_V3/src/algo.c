#include "../lib/algo.h"

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*  P : Sorts the provided array using the Bubble Sort algo */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
int bubbleSort(t_algo_meta *meta){
    void *tmp = meta->tab;

    for(int i=0 ; i<meta->tabsize-1 ; i++){
        for(int j=0 ; j<meta->tabsize-i-1 ; j++){
            //compare tab[i] and tab[i+1]
            if((*meta->compare)(tmp+(meta->elementsize*j), tmp+(meta->elementsize*(j+1))) > 0)
                //swap tab[i] and tab[i+1]
                (*meta->swapping)(tmp+(meta->elementsize*j), tmp+(meta->elementsize*(j+1)));
        }
    }

    return 0;
}

/************************************************************/
/*  I : Meta data necessary to the algorithm                */
/*      Element to search                                   */
/*  P : Searches the key using the Binary search algorithm  */
/*  O : -1  -> Not found                                    */
/*     >= 0 -> Index of the first occurence in the array    */
/************************************************************/
int binarySearch(t_algo_meta *meta, void* toSearch){
    int i=0, j=meta->tabsize-1, m=0;
    void *tmp = meta->tab;

    while(i<=j){
        m = (i+j)/2;
        if((*meta->compare)(tmp+(meta->elementsize*m), toSearch) < 0)
            i = m+1;
        else
            if((*meta->compare)(tmp+(meta->elementsize*m), toSearch) > 0)
                j = m-1;
            else
                return m;
    }

    return -1;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to search                                   */
/*  P : Finds the first occurence of the key using          */
/*          the Binary search algorithm                     */
/*  O : -1  -> Not found                                    */
/*      >=0 -> Index of the first occurence in the array    */
/************************************************************/
int binarySearchFirst(t_algo_meta *meta, void* toSearch){
    void* tmp = meta->tab;

    //use the binary search to find an occurence of the element
    int i = binarySearch(meta, toSearch);

    if(i<0)
        return -1;

    //walk through all the occurences of the key until the first one
    while(i>=0 && (*meta->compare)(tmp+(meta->elementsize*i), toSearch) >= 0){
        i--;
    }
    return i+1;
}

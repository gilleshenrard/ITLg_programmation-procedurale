#include "../lib/algo.h"

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*  P : Sorts the provided array using the Bubble Sort algo */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
int bubbleSort(t_algo_meta *meta){
    void *current=NULL, *next=NULL;

    for(int i=0 ; i<meta->tabsize-1 ; i++){
        for(int j=0 ; j<meta->tabsize-i-1 ; j++){
            //properly place the cursors
            current = meta->tab+(meta->elementsize*j);
            next = meta->tab+(meta->elementsize*(j+1));

            if((*meta->doCompare)(current, next) > 0)
                (*meta->doSwap)(current, next);
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
    void *current = NULL;

    while(i<=j){
        m = (i+j)/2;
        //position the cursor
        current = meta->tab+(meta->elementsize*m);

        if((*meta->doCompare)(current, toSearch) < 0)
            i = m+1;
        else
            if((*meta->doCompare)(current, toSearch) > 0)
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
    void* current = NULL;

    //use the binary search to find an occurence of the element
    int i = binarySearch(meta, toSearch);

    if(i<0)
        return -1;

    //walk through all the occurences of the key until the first one
    do{
        i--;
        current = meta->tab+(meta->elementsize*i);
    }while(i>=0 && (*meta->doCompare)(current, toSearch) >= 0);

    return i+1;
}

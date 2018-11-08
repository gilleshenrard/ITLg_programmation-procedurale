#include "../lib/algo.h"

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*  P : Sorts the provided array using the Bubble Sort algo */
/*  O :  0 -> Sorted                                        */
/*      -1 -> Error                                         */
/************************************************************/
int bubbleSort(t_algo_meta *meta){
    void *current=NULL, *next=NULL;

    //no meta data available
    if(!meta || !meta->doCompare || !meta->doSwap)
        return -1;

    //array is empty
    if(!meta->structure)
        return 0;

    for(int i=0 ; i<meta->nbelements-1 ; i++){
        for(int j=0 ; j<meta->nbelements-i-1 ; j++){
            //properly place the cursors
            current = meta->structure+(meta->elementsize*j);
            next = meta->structure+(meta->elementsize*(j+1));

            if((*meta->doCompare)(current, next) > 0)
                if((*meta->doSwap)(current, next) < 0)
                    return -1;
        }
    }

    return 0;
}

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*  P : Sorts the provided linked list using                */
/*          the Bubble Sort algorithm                       */
/*  O :  0 -> Sorted                                        */
/*      -1 -> Error                                         */
/************************************************************/
int bubbleSortList(t_algo_meta* meta){
    void *current=NULL, *next=NULL, *right_ptr=NULL;
    int swapped;

    //no meta data available
    if(!meta || !meta->doCompare || !meta->doSwap)
        return -1;

    //list is empty
    if(!meta->structure)
        return 0;

    do{
        swapped = 0;
        current = meta->structure;
        next = *(*meta->next)(current);

        while(next != right_ptr){
            if((*meta->doCompare)(current, next) > 0){
                if((*meta->doSwap)(current, next) < 0)
                    return -1;
                swapped = 1;
            }
            current = next;
            next = *(*meta->next)(current);
        }
        right_ptr = current;
    }while(swapped);

    return 0;
}

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*      Lowest element of the partition                     */
/*      Highest element of the partition                    */
/*  P : Sorts the partitions provided by the Quick Sort     */
/*  O : New pivot                                           */
/************************************************************/
/*  WARNING : is solely to be used by the quick sort func.! */
/************************************************************/
int quickSortPartitioning(t_algo_meta* meta, int low, int high){
    void* pivot = meta->structure+(meta->elementsize*high), *elem_i=NULL, *elem_j=NULL;
    int i = low-1;

    //swap the elements until the pivot is at the right place
    //      with lower elements before, and higher ones after
    for(int j=low ; j<=high-1 ; j++){
        elem_j = meta->structure+(meta->elementsize*j);
        if((*meta->doCompare)(elem_j, pivot) <= 0){
            i++;
            elem_i = meta->structure+(meta->elementsize*i);
            (*meta->doSwap)(elem_i, elem_j);
        }
    }

    //place the pivot at the right place by swapping i+1 and high
    //      (uses elem_i and elem_j for the sake of not creating new pointers)
    elem_i = meta->structure+(meta->elementsize*(i+1));
    elem_j = meta->structure+(meta->elementsize*high);
    (*meta->doSwap)(elem_i, elem_j);

    return(i+1);
}

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*      Lowest index in the array (most likely 0)           */
/*      Highest index in the array (last element)           */
/*  P : Sorts the provided array using the Quick Sort algo  */
/*  O :  0 -> Sorted                                        */
/*      -1 -> Error                                         */
/************************************************************/
int quickSort(t_algo_meta* meta, int low, int high){
    int pivot=0;

    //no meta data available
    if(!meta || !meta->doCompare || !meta->doSwap)
        return -1;

    //list is empty
    if(!meta->structure)
        return 0;

    if(low < high){
        pivot = quickSortPartitioning(meta, low, high);

        if(quickSort(meta, low, pivot-1) < 0)
            return -1;
        if(quickSort(meta, pivot+1, high) <0)
            return -1;
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
    int i=0, j=meta->nbelements-1, m=0;
    void *current = NULL;

    while(i<=j){
        m = (i+j)/2;
        //position the cursor
        current = meta->structure+(meta->elementsize*m);

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
        current = meta->structure+(meta->elementsize*i);
    }while(i>=0 && (*meta->doCompare)(current, toSearch) >= 0);

    return i+1;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to append in the list                       */
/*  P : Inserts an element at the top of a linked list      */
/*  O : 0 -> Element added                                  */
/*     -1 -> Error                                          */
/************************************************************/
int insertListTop(t_algo_meta* meta, void *toAdd){
    void *newElement = NULL, **nextelem = NULL;

    //check if meta data available
    if(!meta || !meta->doCopy || !meta->next || !toAdd)
        return -1;

    //memory allocation for the new element
    newElement = malloc(meta->elementsize);
    if(!newElement)
        return -1;

    //copy new element data and organise pointers
    (*meta->doCopy)(newElement, toAdd);
    nextelem = (*meta->next)(newElement);
    *nextelem = meta->structure;
    meta->structure = newElement;

    //increment the elements counter
    meta->nbelements++;

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*  P : Removes the first element of the list               */
/*  O : 0 -> Element popped                                 */
/*     -1 -> Error                                          */
/************************************************************/
int popListTop(t_algo_meta* meta){
    void *head = NULL, *second=NULL;

    //check if meta data available
    if(!meta || !meta->next)
        return -1;

    //Structure is empty
    if(!meta->structure)
        return 0;

    //save list head and retrieve next element
    head = meta->structure;
    second = *(*meta->next)(head);

    //free and rechain
    //  note : free() takes a void pointer anyway, so no need to cast
    free(head);
    meta->structure = second;

    //update the number of elements
    meta->nbelements -= 1;

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to insert in the list                       */
/*  P : Inserts an element at the right place in a sorted   */
/*          linked list                                     */
/*  O : 0 -> Element added                                  */
/*     -1 -> Error                                          */
/************************************************************/
int insertListSorted(t_algo_meta *meta, void* toAdd){
    void *newElement = NULL, *previous=NULL, *current=meta->structure, **next = NULL;

    //non-existing list or element is supposed to become the first element
    if(!meta->structure || (*meta->doCompare)(meta->structure, toAdd) > 0)
        return insertListTop(meta, toAdd);

    //allocation and filling of the new element
    newElement = malloc(meta->elementsize);
    if(newElement)
        (*meta->doCopy)(newElement, toAdd);
    else
        return -1;

    //walk through the list until the right place is found
    while(current!=NULL && (*meta->doCompare)(newElement,current)>0){
        previous = current;
        current = *(*meta->next)(current);
    }
    //properly link the elements
    next = (*meta->next)(previous);
    *next = newElement;
    next = (*meta->next)(newElement);
    *next = current;

    meta->nbelements++;

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Parameter for the action to perform                 */
/*      Action to perform                                   */
/*  P : Performs an action on every element of the list     */
/*  O : 0 -> Element added                                  */
/*     -1 -> Error                                          */
/************************************************************/
int foreachList(t_algo_meta* meta, void* parameter, int (*doAction)(void*, void*)){
    void *next = NULL, *current=NULL;

    if(!meta || !meta->next || !doAction)
        return -1;

    next = meta->structure;

    while(next){
        current = next;
        next = *(*meta->next)(next);
        if((*doAction)(current, parameter) < 0)
            return -1;
    }

    return 0;
}

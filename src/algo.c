#include "../lib/algo.h"

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*  P : Sorts the provided array using the Bubble Sort algo */
/*  O : /                                                   */
/************************************************************/
void bubbleSort(t_algo_meta *meta){
    void *current=NULL, *next=NULL;

    for(int i=0 ; i<meta->nbelements-1 ; i++){
        for(int j=0 ; j<meta->nbelements-i-1 ; j++){
            //properly place the cursors
            current = meta->structure+(meta->elementsize*j);
            next = meta->structure+(meta->elementsize*(j+1));

            if((*meta->doCompare)(current, next) > 0)
                (*meta->doSwap)(current, next);
        }
    }
}

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*      First element of the list (head)                    */
/*  P : Sorts the provided linked list using                */
/*          the Bubble Sort algorithm                       */
/*  O : /                                                   */
/************************************************************/
void bubbleSortList(t_algo_meta* meta, void** first){
    void *current=NULL, *next=NULL, *right_ptr=NULL;
    int swapped;

    if(!*first)
        return;

    do{
        swapped = 0;
        current = *first;
        next = (*meta->next)(current);

        while(next != right_ptr){
            if((*meta->doCompare)(current, next) > 0){
                (*meta->doSwap)(current, next);
                swapped = 1;
            }
            current = next;
            next = (*meta->next)(current);
        }
        right_ptr = current;
    }while(swapped);
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
/*  O : /                                                   */
/************************************************************/
void quickSort(t_algo_meta* meta, int low, int high){
    int pivot=0;

    if(low < high){
        pivot = quickSortPartitioning(meta, low, high);

        quickSort(meta, low, pivot-1);
        quickSort(meta, pivot+1, high);
    }
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
/*      Head (first element) of the list                    */
/*      Element to append in the list                       */
/*  P : Inserts an element at the top of a linked list      */
/*  O : 0 -> Element added                                  */
/*     -1 -> Error                                          */
/************************************************************/
int insertListTop(t_algo_meta* meta, void **first, void *toAdd){
    void *newElement = NULL, ** nextelem = NULL;

    newElement = malloc(meta->elementsize);
    if(!newElement)
        return -1;

    (*meta->doCopy)(newElement, toAdd);
    nextelem = (*meta->next)(newElement);
    *nextelem = *first;
    *first = newElement;

    return 0;
}
/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Head (first element) of the list                    */
/*      Element to insert in the list                       */
/*  P : Inserts an element at the right place in a sorted   */
/*          linked list                                     */
/*  O : 0 -> Element added                                  */
/*     -1 -> Error                                          */
/************************************************************/
int insertListSorted(t_algo_meta *meta,  void** first, void* toAdd){
    void *newElement = NULL, *previous=NULL, *current=*first, **next = NULL;

    //non-existing list (use push function)
    if(!*first)
        return insertListTop(meta, first, toAdd);

    //allocation and filling of the new element
    newElement = malloc(meta->elementsize);
    if(newElement)
        (*meta->doCopy)(newElement, toAdd);
    else
        return -1;

    //new element is to go at the beginning of the list
    if((*meta->doCompare)(newElement, current) <= 0 ){
        next = (*meta->next)(newElement);
        *next = *first;
        *first = newElement;
    }
    else{
        //walk through the list until the right place is found
        while(current!=NULL && (*meta->doCompare)(newElement,current)>0){
            previous = current;
            next = (*meta->next)(current);
            current = *next;
        }
        //properly link the elements
        next = (*meta->next)(previous);
        *next = newElement;
        next = (*meta->next)(newElement);
        *next = current;
    }
    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Head (first element) of the list                    */
/*      Parameter for the action to perform                 */
/*      Action to perform                                   */
/*  P : Performs an action on every element of the list     */
/*  O : /                                                   */
/************************************************************/
void foreachList(t_algo_meta* meta, void **first, void* parameter, int (*doAction)(void*, void*)){
    void *cur = *first, *prev=NULL;
    void** nextelem=NULL;

    do{
        prev = cur;
        nextelem = (*meta->next)(cur);
        cur = *nextelem;
        (*doAction)(prev, parameter);
    }while(cur);
}

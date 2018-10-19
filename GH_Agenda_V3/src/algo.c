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
/*  I : Array of meta data necessary to the algorithm       */
/*      Lowest element of the partition                     */
/*      Highest element of the partition                    */
/*  P : Sorts the partitions provided by the Quick Sort     */
/*  O : New pivot                                           */
/************************************************************/
/*  WARNING : is solely to be used by the quick sort func.! */
/************************************************************/
int quickSortPartitioning(t_algo_meta* meta, int low, int high){
    void* pivot = meta->tab+(meta->elementsize*high), *elemi=NULL, *elemj=NULL;
    int i = low-1;

    //swap the elements until the pivot is at the right place
    //      with lower elements before, and higher ones after
    for(int j=low ; j<=high-1 ; j++){
        elemj = meta->tab+(meta->elementsize*j);
        if((*meta->doCompare)(elemj, pivot) <= 0){
            i++;
            elemi = meta->tab+(meta->elementsize*i);
            (*meta->doSwap)(elemi, elemj);
        }
    }

    //pivot is at the right place and partition sorted
    (*meta->doSwap)(meta->tab+(meta->elementsize*(i+1)), meta->tab+(meta->elementsize*high));
    return(i+1);
}

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*      Lowest index in the array (most likely 0)           */
/*      Highest index in the array (last element)           */
/*  P : Sorts the provided array using the Quick Sort algo  */
/*  O : /                                                   */
/************************************************************/
int quickSort(t_algo_meta* meta, int low, int high){
    int pivot=0;

    if(low < high){
        pivot = quickSortPartitioning(meta, low, high);

        quickSort(meta, low, pivot-1);
        quickSort(meta, pivot+1, high);
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


/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Head (first element) of the list                    */
/*      Queue (last element) of the list                    */
/*      Element to append in the list                       */
/*      Function to get the next element of an element      */
/*  P : Append the element to the unsorted list             */
/*  O : 0 -> Element added                                  */
/*     -1 -> Error                                          */
/************************************************************/
int appendUnsortedList(t_list_meta* listmeta, void **first, void **last, void *toAdd){
    void *newElement = NULL, *tmp = NULL;
    void** nextelem = NULL;

        //Allocate memory for the new element
    newElement = malloc(listmeta->meta.elementsize);
    if(!newElement)
        return -1;

    //Copy the values of the element to add in the new element
    (*listmeta->doAssign)(newElement, toAdd);

    tmp = *first;
    if(!tmp){
        //list is empty -> create
        *first = newElement;
        *last = newElement;
    }
    else{
        //append at the end
        tmp = *last;
        nextelem = (*listmeta->next)(tmp);
        *nextelem = newElement;
        *last = newElement;
    }

    return 0;
}

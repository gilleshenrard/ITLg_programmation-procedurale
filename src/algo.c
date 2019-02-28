#include "../lib/algo.h"

/************************************************************/
/*  I : List to copy                                        */
/*      Array to create (MUST BE EMPTY)                     */
/*      Action to perform on the list members (free or not) */
/*  P : Allocates memory for the array and copies the list  */
/*          into it                                         */
/*  O :  0 -> Array created                                 */
/*      -1 -> Error                                         */
/************************************************************/
int listToArray(t_algo_meta* dList, t_algo_meta* dArray, e_listtoarray action){
    void *tmp_array = NULL, *tmp_list = NULL;

    //check if the array doesn't exist
    if(dArray->structure)
        return -1;

    //allocate the memory
    dArray->structure = calloc(dList->nbelements, dList->elementsize);
    if(!dArray)
        return -1;

    dArray->nbelements = dList->nbelements;

    //copy elements one by one in the array
    tmp_list = dList->structure;
    for(int i=0 ; i<dArray->nbelements ; i++){
        //position the pointer properly
        tmp_array = dArray->structure+(dArray->elementsize*i);
        if((*dList->doCopy)(tmp_array, tmp_list) <0)
            return -1;

        //if desired, free the freshly copied element
        if(action == REPLACE){
            if(popListTop(dList) <0){
                return -1;
            }
            tmp_list = dList->structure;
        }
        else{
            //increment the list pointer
            tmp_list = *(*dList->next)(tmp_list);
        }

    }

    return 0;
}

/************************************************************/
/*  I : Array to copy                                       */
/*      List to create                                      */
/*      Action to perform on the list members (free or not) */
/*  P : Allocates memory for the list and copies the array  */
/*          into it                                         */
/*  O :  0 -> List created                                  */
/*      -1 -> Error                                         */
/************************************************************/
int arrayToList(t_algo_meta* dArray, t_algo_meta* dList, e_listtoarray action){
    void *tmp_array = dArray->structure;

    //copy elements one by one in the list
    for(int i=0 ; i<dArray->nbelements ; i++){
        //position the pointer properly
        tmp_array = dArray->structure+(dArray->elementsize*i);
        //insert in the list
        if(insertListSorted(dList,  tmp_array) < 0)
            return -1;
    }

    return 0;
}

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
    if(!meta || !meta->doCompare || !meta->doSwap || !meta->next)
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
    void *newElement = NULL, **nextelem = NULL, **previousElem = NULL;

    //check if meta data available
    if(!meta || !meta->doCopy || !meta->next || !meta->previous || !toAdd)
        return -1;

    //memory allocation for the new element (calloc to initialize with all 0)
    newElement = calloc(1, meta->elementsize);
    if(!newElement)
        return -1;

    //copy new element data
    (*meta->doCopy)(newElement, toAdd);

    //chain new element's previous pointer to list, if existing
    if(meta->structure){
        //chain new element's next pointer to list
        nextelem = (*meta->next)(newElement);
        *nextelem = meta->structure;
        //chain list's head previous pointer to new element
        previousElem = (*meta->previous)(meta->structure);
        *previousElem = newElement;
    }

    //make the new element head of the list
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
    void *head = NULL, *second=NULL, **previous = NULL;

    //check if meta data available
    if(!meta || !meta->next)
        return -1;

    //Structure is empty
    if(!meta->structure)
        return 0;

    //save list head and retrieve next element
    head = meta->structure;
    second = *(*meta->next)(head);
    previous = (*meta->previous)(second);

    //free and rechain
    //  note : free() takes a void pointer anyway, so no need to cast
    free(head);
    if(previous)
        *previous = NULL;
    meta->structure = second;

    //update the number of elements
    meta->nbelements--;

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
    void *newElement = NULL, *previous=NULL, *next=meta->structure, **tmp = NULL;

    //non-existing list or element is supposed to become the first element
    if(!meta->structure || (*meta->doCompare)(toAdd, meta->structure) <= 0)
        return insertListTop(meta, toAdd);

    //allocation and filling of the new element
    newElement = malloc(meta->elementsize);
    if(newElement)
        (*meta->doCopy)(newElement, toAdd);
    else
        return -1;

    //walk through the list until the right place is found
    while(next!=NULL && (*meta->doCompare)(newElement,next)>0){
        previous = next;
        next = *(*meta->next)(next);
    }
    //previous->next = new
    tmp = (*meta->next)(previous);
    if(tmp)
        *tmp = newElement;
    //new->next = next
    tmp = (*meta->next)(newElement);
    *tmp = next;
    //new->previous = previous
    tmp = (*meta->previous)(newElement);
    *tmp = previous;
    //next->previous = new
    if(*(*meta->next)(newElement) != NULL){
        tmp = (*meta->previous)(next);
        *tmp = newElement;
    }

    meta->nbelements++;

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Parameter for the action to perform                 */
/*      Action to perform                                   */
/*  P : Performs an action on every element of the list     */
/*  O : 0 -> OK                                             */
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

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Parameter for the action to perform                 */
/*      Action to perform                                   */
/*  P : Performs an action on every element of the array    */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
int foreachArray(t_algo_meta* meta, void* parameter, int (*doAction)(void*, void*)){
    void* tmp = NULL;

    for(int i=0 ; i<meta->nbelements ; i++){
        //position the pointer properly
        tmp = meta->structure+(meta->elementsize*i);
        //execute action
        if((*doAction)(tmp, parameter) < 0)
            return -1;
    }

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to insert in the AVL                        */
/*  P : Inserts an element in an AVL                        */
/*  O : AVL root if ok                                      */
/*      NULL otherwise                                      */
/************************************************************/
void* insertAVL(t_algo_meta* meta, void* avl, void* toAdd){
    void** childitem=NULL;

    if(!avl){
        //memory allocation for the new element (calloc to initialize with all 0)
        avl = calloc(1, meta->elementsize);
        if(!avl)
            return NULL;

        //copy new element data
        (*meta->doCopy)(avl, toAdd);
        return avl;
    }

    //sort whether the new element goes as right or left child
    if((*meta->doCompare)(avl, toAdd) != 0){
        if((*meta->doCompare)(avl, toAdd) < 0)
            childitem = (*meta->next)(avl);
        else
            childitem = (*meta->previous)(avl);

        //build a new AVL with the child as root
        *childitem = insertAVL(meta, *childitem, toAdd);
    }

    return avl;
}

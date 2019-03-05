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
/*  I : Array to copy                                       */
/*      AVL to create                                       */
/*      Action to perform on the list members (free or not) */
/*  P : Allocates memory for the AVL and copies the array   */
/*          into it                                         */
/*  O :  0 -> AVL created                                   */
/*      -1 -> Error                                         */
/************************************************************/
int arrayToAVL(t_algo_meta* dArray, t_algo_meta* dAVL, e_listtoarray action){
    void *tmp_array = dArray->structure;

    //copy elements one by one in the list
    for(int i=0 ; i<dArray->nbelements ; i++){
        //position the pointer properly
        tmp_array = dArray->structure+(dArray->elementsize*i);
        //insert in the AVL
        dAVL->structure = insertAVL(dAVL, dAVL->structure, tmp_array);
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

    //memory allocation for the new element
    newElement = (*meta->doCreate)();
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
    void **child_right=NULL, **child_left=NULL;
    int height_left=0, height_right=0, balance=0;

    //if tree is empty
    if(!avl){
        //memory allocation for the new element
        avl = (*meta->doCreate)();
        if(!avl)
            return NULL;

        //copy new element data
        (*meta->doCopy)(avl, toAdd);
        (*meta->setHeight)(avl, 1);
        meta->nbelements++;
        return avl;
    }

    //prepare pointers to the left and right children
    child_right = (*meta->next)(avl);
    child_left = (*meta->previous)(avl);

    //sort whether the new element goes as right or left child
    //  + build a new AVL with the child as root
    if((*meta->doCompare)(avl, toAdd) != 0){
        if((*meta->doCompare)(avl, toAdd) < 0)
            *child_right = insertAVL(meta, *child_right, toAdd);
        else
            *child_left = insertAVL(meta, *child_left, toAdd);
    }
    else{
        //ignore duplicates (forbidden in AVL trees)
        return avl;
    }

    //get the height of the left and right children AVL
    height_right = (*meta->getHeight)(*child_right);
    height_left = (*meta->getHeight)(*child_left);

    //update the current node's height
    (*meta->setHeight)(avl, 1+(height_left > height_right ? height_left : height_right));

    if(avl)
        balance = height_left - height_right;

    if(balance < -1){
        // right right case
        if((*meta->doCompare)(*child_right, toAdd) < 0){
            return rotate_AVL(meta, avl, LEFT);
        }
        // right left case
        if((*meta->doCompare)(*child_right, toAdd) > 0){
            *child_right = rotate_AVL(meta, child_right, RIGHT);
            return rotate_AVL(meta, avl, LEFT);
        }
    }
    if(balance > 1){
        // left left case
        if((*meta->doCompare)(*child_left, toAdd) > 0){
            return rotate_AVL(meta, avl, RIGHT);
        }
        //left right case
        if((*meta->doCompare)(*child_left, toAdd) < 0){
            *child_left = rotate_AVL(meta, *child_left, LEFT);
            return rotate_AVL(meta, avl, RIGHT);
        }
    }

    return avl;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to browse/display                           */
/*      Character designed to be displayed to indicate      */
/*          node direction compared to its root             */
/*      Method to get the string ID of the node             */
/*  P : Displays an AVL as a tree                           */
/*  O : /                                                   */
/************************************************************/
void display_AVL_tree(t_algo_meta* meta, void* avl, char dir, char* (*toString)(void*)){
    char tmp[80]={0};
    int height = (*meta->getHeight)(avl);
    void** child_left=(*meta->previous)(avl);
    void** child_right=(*meta->next)(avl);
    int nbc_pad;

    offset_max = ++offset > offset_max ? offset : offset_max;

    if(avl){
        display_AVL_tree(meta, *child_left, 'L', toString);

        nbc_pad = LG_MAX - (3 * offset) - strlen((*toString)(avl));
        for (int i=0;i<nbc_pad;i++)
            strcat(tmp,".");
        strcat(tmp,(*toString)(avl));
        printf("%*c%c %s R-%p R-%p L-%p H-%d\n", 3*offset, '-', dir, tmp, avl, *child_left, *child_right, height);

        display_AVL_tree(meta, *child_right, 'R', toString);
    }
    offset--;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      AVL tree to rotate                                  */
/*      Side of the rotation (LEFT or RIGHT)                */
/*  P : Rotates an AVL to the side required                 */
/*  O : Rotated AVL                                         */
/************************************************************/
void* rotate_AVL(t_algo_meta* meta, void* avl, e_rotation side){
    void** (*normally_left)(void*);
    void** (*normally_right)(void*);
    void **child_left = NULL, **child_right=NULL;
    void *newTree=NULL, *rightLeaf=NULL;
    int height_l=0, height_r=0, height=0;


    //invert the function pointers to get the right child
    //  depending on the side of the rotation
    normally_left = (side == RIGHT ? meta->previous : meta->next);
    normally_right = (side == RIGHT ? meta->next : meta->previous);

    //prepare pointers for the new tree
    child_left = (*normally_left)(avl);
    newTree = *child_left;
    child_right = (*normally_right)(newTree);
    //problem here (11th node added in unit test)
    rightLeaf = *child_right;

    //perform rotation
    child_right = (*normally_right)(newTree);
    *child_right = avl;
    child_left = (*normally_left)(avl);
    *child_left = rightLeaf;

    //set new height of the previous root
    child_left = (*meta->previous)(avl);
    child_right = (*meta->next)(avl);
    height_l = (*meta->getHeight)(*child_left);
    height_r = (*meta->getHeight)(*child_right);
    height = (height_l > height_r ? height_l : height_r) + 1;
    (*meta->setHeight)(avl, height);

    //set new height of the new root
    child_left = (*meta->previous)(newTree);
    child_right = (*meta->next)(newTree);
    height_l = (*meta->getHeight)(*child_left);
    height_r = (*meta->getHeight)(*child_right);
    height = (height_l > height_r ? height_l : height_r) + 1;
    (*meta->setHeight)(newTree, height);

    return newTree;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      AVL tree of which to compute the balance            */
/*  P : Computes and returns the balance of an AVL          */
/*  O : Balance                                             */
/************************************************************/
int get_AVL_balance(t_algo_meta* meta, void* avl){
    int height_left=0, height_right=0;
    void** childitem = NULL;

    if(!avl)
        return 0;

    childitem = (*meta->next)(avl);
    height_right = (*meta->getHeight)(*childitem);
    childitem = (*meta->previous)(avl);
    height_left = (*meta->getHeight)(*childitem);

    return height_left - height_right;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Root of the AVL to which perform the action         */
/*      Parameter for the action to perform                 */
/*      Action to perform                                   */
/*  P : Performs an action on every element of the AVL      */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
int foreachAVL(t_algo_meta* meta, void* avl, void* parameter, int (*doAction)(void*, void*)){
    void **child_left=NULL, **child_right=NULL;
    int ret = 0;

    if(avl){
        //get the left and right children of the current root
        child_left = (*meta->previous)(avl);
        child_right = (*meta->next)(avl);

        //perform action on left child
        foreachAVL(meta, *child_left, parameter, doAction);

        //perform action on root
        ret = (*doAction)(avl, parameter);

        //perform action on right child
        foreachAVL(meta, *child_right, parameter, doAction);
    }

    return ret;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      AVL in which search for the key                     */
/*      Key to search in the AVL                            */
/*  P : Recursively search for a key in the AVL             */
/*  O : Leaf if found                                       */
/*      NULL otherwise                                      */
/************************************************************/
void* search_AVL(t_algo_meta* meta, void* avl, void* key){
    void **child = NULL;

    // if found or not existing, return the result
    if(avl == NULL || (*meta->doCompare)(avl, key) == 0)
        return avl;

    // otherwise, decide whether going to the right or left child
    if((*meta->doCompare)(avl, key) < 0)
        child = (*meta->next)(avl);
    else
        child = (*meta->previous)(avl);

    // perform the search in the sub-child
    return search_AVL(meta, *child, key);
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Root of the AVL from which remove an elemnt         */
/*      Key to remove from the AVL                          */
/*  P : Removes an element from the AVL provided            */
/*  O : Leaf if found                                       */
/*      NULL otherwise                                      */
/************************************************************/
void* delete_AVL(t_algo_meta* meta, void* root, void* key){
    void **child_left=NULL, **child_right=NULL;
    void **tmp_cl=NULL, **tmp_cr=NULL;
    int height_right=0, height_left=0, balance=0;
    void *tmp=NULL;

    if(!root)
        return root;

    child_right = (*meta->next)(root);
    child_left = (*meta->previous)(root);

    if((*meta->doCompare)(root, key) < 0)
        *child_right = delete_AVL(meta, *child_right, key);
    else if((*meta->doCompare)(root, key) > 0)
        *child_left = delete_AVL(meta, *child_left, key);
    else{
        if(*child_left==NULL || *child_right==NULL){
            tmp = (*child_left ? *child_left : *child_right);

            if(!tmp){
                tmp = root;
                root = NULL;
            }
            else{
                (*meta->doCopy)(root, tmp);
                *(*meta->next)(root) = *(*meta->next)(tmp);
                *(*meta->previous)(root) = *(*meta->previous)(tmp);
            }

            free(tmp);
            meta->nbelements--;
        }
        else{
            tmp_cr = (*meta->next)(*child_right);
            tmp_cl = (*meta->previous)(*child_right);

            tmp = ((*meta->doCompare)(*tmp_cl, *tmp_cr) < 0 ? *tmp_cl : *tmp_cr);

            (*meta->doCopy)(root, tmp);

            *child_right = delete_AVL(meta, *child_right, tmp);
        }
    }

    if(!root)
        return root;

    //get the height of the left and right children AVL
    height_right = (*meta->getHeight)(*child_right);
    height_left = (*meta->getHeight)(*child_left);

    //update the current node's height
    (*meta->setHeight)(root, 1+(height_left > height_right ? height_left : height_right));

    if(root)
        balance = ((*meta->getHeight)(*(*meta->previous)(root))) - ((*meta->getHeight)(*(*meta->next)(root)));

    if(balance < -1){
        tmp_cr = (*meta->next)(*child_right);
        tmp_cl = (*meta->previous)(*child_right);
        height_right = (*meta->getHeight)(*tmp_cr);
        height_left = (*meta->getHeight)(*tmp_cl);
        balance = height_left - height_right;

        // right right case
        if(balance <= 0){
            return rotate_AVL(meta, root, LEFT);
        }
        // right left case
        if(balance > 0){
            *child_right = rotate_AVL(meta, *child_right, RIGHT);
            return rotate_AVL(meta, root, LEFT);
        }
    }
    if(balance > 1){
        tmp_cr = (*meta->next)(*child_left);
        tmp_cl = (*meta->previous)(*child_left);
        height_right = (*meta->getHeight)(*tmp_cr);
        height_left = (*meta->getHeight)(*tmp_cl);
        balance = height_left - height_right;

        // left left case
        if(balance >= 0){
            return rotate_AVL(meta, root, RIGHT);
        }
        //left right case
        if(balance < 0){
            *child_left = rotate_AVL(meta, *child_left, LEFT);
            return rotate_AVL(meta, root, RIGHT);
        }
    }

    return root;
}

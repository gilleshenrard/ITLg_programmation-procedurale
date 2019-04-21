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
    newElement = calloc(1, meta->elementsize);;
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
    newElement = calloc(1, meta->elementsize);
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
        avl = calloc(1, meta->elementsize);
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
        printf("%*c%c %s T-%p R-%p L-%p H-%d\n", 3*offset, '-', dir, tmp, avl, *child_right, *child_left, height);

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
    int height_right=0, height_left=0, balance=0;
    void *tmp=NULL;

    //if no AVL, skip
    if(!root)
        return root;

    //get the address of the current AVL children
    child_right = (*meta->next)(root);
    child_left = (*meta->previous)(root);

    //key in the right subtree
    if((*meta->doCompare)(root, key) < 0)
        *child_right = delete_AVL(meta, *child_right, key);

    //key in the left subtree
    else if((*meta->doCompare)(root, key) > 0)
        *child_left = delete_AVL(meta, *child_left, key);

    //key is found
    else{
        //node with less than 2 children nodes
        if(*child_left==NULL || *child_right==NULL){
            //get at least one existing child
            tmp = (*child_left ? *child_left : *child_right);

            //no children nodes
            if(!tmp){
                tmp = root;
                root = NULL;
            }
            //one child node
            else{
                //copy the child node in the father node (with addresses of the grand-children)
                (*meta->doCopy)(root, tmp);
                *(*meta->next)(root) = *(*meta->next)(tmp);
                *(*meta->previous)(root) = *(*meta->previous)(tmp);
            }

            //free the memory of the father
            free(tmp);
            meta->nbelements--;
        }
        else{
            //2 children nodes : copy the child with the smallest value in the root,
            //                      then delete it
            tmp = min_AVL_value(meta, *child_right);
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

    //if still a root, re-balance accordingly
    if(root)
        balance = get_AVL_balance(meta, root);

    if(balance < -1){
        // right right case
        if(get_AVL_balance(meta, *child_right) <= 0){
            return rotate_AVL(meta, root, LEFT);
        }
        // right left case
        if(get_AVL_balance(meta, *child_right) > 0){
            *child_right = rotate_AVL(meta, *child_right, RIGHT);
            return rotate_AVL(meta, root, LEFT);
        }
    }
    if(balance > 1){
        // left left case
        if(get_AVL_balance(meta, *child_left) >= 0){
            return rotate_AVL(meta, root, RIGHT);
        }
        //left right case
        if(get_AVL_balance(meta, *child_left) < 0){
            *child_left = rotate_AVL(meta, *child_left, LEFT);
            return rotate_AVL(meta, root, RIGHT);
        }
    }

    return root;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*  P : Removes the root from the AVL provided              */
/*  O :  0 if OK                                            */
/*      -1 otherwise                                        */
/************************************************************/
int delete_AVL_root(t_algo_meta* meta){
    meta->structure = delete_AVL(meta, meta->structure, meta->structure);

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      AVL for which find the smallest value               */
/*  P : Finds the subtree with the smallest value           */
/*          (most to the left)                              */
/*  O : Most left node in the subtree                       */
/************************************************************/
void* min_AVL_value(t_algo_meta* meta, void* avl){
    void* current = avl;
    void** tmp = (*meta->previous)(current);

    while(*tmp){
        current = *tmp;
        tmp = (*meta->previous)(current);
    }

    return current;
}

/************************************************************/
/*  I : File pointer to the database                        */
/*      Offset of the first element of the data block       */
/*      Number of elements in the data block                */
/*      Metadata necessary to the algorithm                 */
/*  P : Assuming the block is filled with a known number of */
/*          elements, the algo will chain its members as a  */
/*          binary tree (changes the pointers only)         */
/*  O : Offset of the current tree root                     */
/************************************************************/
/* WARNING : the index structure must finish with left and  */
/*              right long int types                        */
/************************************************************/
long index_tree(FILE* fp, long offset_start, int nb, t_algo_meta* meta){
    long old_offset=0, root=0, subtree=0;
    int nb_g=0, nb_d=0;

    //save the previous tree root offset
    old_offset = ftell(fp);

    //define the number of elements total (-1 in case of even number)
    //  and the number of elements on left and right
    nb_g = (nb-1)/2;
    nb_d = (nb-1) - nb_g;

    if(nb_g > 0){
        //set the file pointer to the "left child" field of the current root in the disk
        fseek(fp, offset_start + (nb_g*meta->elementsize) + (meta->elementsize - 2*sizeof(long)), SEEK_SET);

        //define the left child offset and write it in the proper field of the current root
        subtree = index_tree(fp, offset_start, nb_g, meta);
        fwrite(&subtree, sizeof(long), 1, fp);
    }
    if(nb_d > 0){
        //set the file pointer to the "right child" field of the current root in the disk
        fseek(fp, offset_start + (nb_g*meta->elementsize) + (meta->elementsize - sizeof(long)), SEEK_SET);

        //define the right child offset and write it in the proper field of the current root
        subtree = index_tree(fp, offset_start + (nb_g+1)*meta->elementsize, nb_d, meta);
        fwrite(&subtree, sizeof(long), 1, fp);
    }

    //get the offset of the current root
    fseek(fp, offset_start + nb_g*meta->elementsize, SEEK_SET);
    root = ftell(fp);

    //restore the previous tree root offset
    fseek(fp, old_offset, SEEK_SET);

    return root;
}

/************************************************************/
/*  I : File pointer to the database                        */
/*      Offset of the index tree root                       */
/*      Key to search in the index                          */
/*      Metadata necessary to the index algorithm           */
/*      Metadata necessary to the list algorithm            */
/*      Size of an element in the table                     */
/*  P : Searches for the key in the index and add all       */
/*          matching elements in a list                     */
/*  O :  0 if OK                                            */
/*      -1 otherwise                                        */
/************************************************************/
/* WARNING : the index structure must finish with left and  */
/*              right long int types                        */
/************************************************************/
int searchall_index(FILE* fp, long offset_root, void* key, t_algo_meta* index, t_algo_meta* list, int elem_size){
    void *index_buf=NULL, *table_buf=NULL;
    int comparison = 0;
    long offset = 0;

    //fill a buffer with the element of the current tree root
    index_buf = calloc(1, index->elementsize);
    fseek(fp, offset_root, SEEK_SET);
    fread(index_buf, 1, index->elementsize, fp);

    //compare it to the key received
    comparison = (*index->doCompare)(index_buf, key);
    if(!comparison){
        //get the offset of the corresponding element in the table
        fseek(fp, offset_root + (index->elementsize - 3*sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);

        //read the corresponding element and add it to the list
        fseek(fp, offset, SEEK_SET);
        table_buf = calloc(1, elem_size);
        fread(table_buf, 1, elem_size, fp);
        insertListSorted(list, table_buf);
        free(table_buf);
    }

    //perform the search in the left subtree
    if(comparison >= 0){
        fseek(fp, offset_root + (index->elementsize - 2*sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);
        if(offset)
            searchall_index(fp, offset, key, index, list, elem_size);
    }

    //perform the search in the right subtree
    if(comparison <= 0){
        fseek(fp, offset_root + (index->elementsize - sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);
        if(offset)
            searchall_index(fp, offset, key, index, list, elem_size);
    }

    free(index_buf);
    return 0;
}

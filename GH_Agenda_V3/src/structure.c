#include "../lib/structure.h"

/************************************************************/
/*  I : Head (first element) of the list                    */
/*      Queue (last element) of the list                    */
/*      Element to append in the list                       */
/*  P : Append the element to the unsorted list             */
/*  O : 0 -> Element added                                  */
/*     -1 -> Error                                          */
/************************************************************/
int appendList(t_tuple **first, t_tuple **last, t_tuple *toAdd){
    t_tuple *newElement = NULL;
    t_tuple *tmp = NULL;

    //Allocate memory for the new element
    newElement = (t_tuple*)malloc(sizeof(t_tuple));
    if(!newElement){
        fprintf(stderr, "\nappendList : memory error");
        return -1;
    }

    //Copy the values of the element to add in the new element
    *newElement = *toAdd;

    tmp = *first;
    if(!tmp){
        //list is empty -> create
        *first = newElement;
        *last = newElement;
    }
    else{
        //append at the end
        tmp = *last;
        tmp->next = newElement;
        *last = newElement;
    }

    return 0;
}

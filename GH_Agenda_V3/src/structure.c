#include "../lib/structure.h"

int appendList(t_tuple **first, t_tuple **last, t_tuple *toAdd){
    t_tuple *newElement = NULL;
    t_tuple *tmp = NULL;

    newElement = (t_tuple*)malloc(sizeof(t_tuple));
    if(!newElement)
        return -1;

    *newElement = *toAdd;

    tmp = *first;
    if(!tmp){
        *first = newElement;
        *last = newElement;
    }
    else{
        tmp = *last;
        tmp->next = newElement;
        *last = newElement;
    }

    return 0;
}

#include "../lib/algo.h"

int bubbleSort(void* tab, int tabsize, int elementsize, int (*compare)(void*, void*), int (*swapping)(void*, void*)){
    void *tmp = tab;

    for(int i=0 ; i<tabsize-1 ; i++){
        for(int j=0 ; j<tabsize-i-1 ; j++){
            if((*compare)(tmp+(elementsize*j), tmp+(elementsize*(j+1))) > 0)
                (*swapping)(tmp+(elementsize*j), tmp+(elementsize*(j+1)));
        }
    }

    return 0;
}

int binarySearch(void* tab, int tabsize, int elementsize, int (*compare)(void*, void*), void* toSearch){
    int i=0, j=tabsize-1, m=0;
    void *tmp = tab;

    while(i<=j){
        m = (i+j)/2;
        if((*compare)(tmp+(elementsize*m), toSearch) < 0)
            i = m+1;
        else
            j = m-1;
    }

    if((*compare)(tmp+(elementsize*m), toSearch) != 0)
        return -1;
    else
        return i;
}

#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
#include <stdio.h>
#include <string.h>

typedef struct t_tuple t_tuple;

struct t_tuple{
    int id;
    char lastname[28];
    char firstname[32];
    char city[32];
    char filler[28];
    t_tuple *next;
};

typedef enum {ID, LASTNAME, FIRSTNAME, CITY} e_critera;

int newTuple(t_tuple*);
int compareLastName(void*, void*);
int compareFilterLastName(void*, void*);
int compareID(void*, void*);
int swapTuples(void*, void*);

#endif // GLOBAL_H_INCLUDED

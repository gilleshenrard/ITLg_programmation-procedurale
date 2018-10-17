#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
#include <stdio.h>
#include <string.h>

typedef struct{
    int id;
    char lastname[28];
    char firstname[32];
    char city[32];
    char filler[32];
} t_tuple;

int newTuple(t_tuple*);

#endif // GLOBAL_H_INCLUDED

#ifndef TUPLE_H_INCLUDED
#define TUPLE_H_INCLUDED
#include <stdio.h>
#include <string.h>

typedef struct t_tuple t_tuple;

struct t_tuple{
    int id;
    char lastname[28];
    char firstname[32];
    char city[32];
    char job[28];
    t_tuple *next;
};

typedef enum {ID, LASTNAME, FIRSTNAME, CITY, JOB} e_criteria;

void displayTuple(t_tuple*);
int encodeTuple(t_tuple*);
int compareLastName(void*, void*);
int compareFilterLastName(void*, void*);
int compareFirstName(void*, void*);
int compareCity(void*, void*);
int compareJob(void*, void*);
int compareID(void*, void*);
int swapTuples(void*, void*);
int assignTuples(void*, void*);
void** nextTuple(void*);

#endif // TUPLE_H_INCLUDED

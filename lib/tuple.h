#ifndef TUPLE_H_INCLUDED
#define TUPLE_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct t_tuple t_tuple;

struct t_tuple{
    int id;
    char firstname[28];
    char lastname[32];
    char birthdate[11];
    char phone[14];
    char mobile[13];
    char email[64];
    char postcode[5];
    char city[32];
    char update[11];
    char filler[34];
    t_tuple *next;
    t_tuple *previous;
};

typedef enum {ID, LASTNAME, FIRSTNAME, BIRTHDATE, PHONE, MOBILE, EMAIL, POSTCODE, CITY, UPDATE} e_criteria;

int displayTupleInline(void*, void*);
int displayTupleBlock(void*, void*);
int encodeTuple(t_tuple*);
int compareLastName(void*, void*);
int compareFilterLastName(void*, void*);
int compareFirstName(void*, void*);
int compareCity(void*, void*);
int compareID(void*, void*);
int swapTuples(void*, void*);
int assignTuples(void*, void*);
void** nextTuple(void*);
void** previousTuple(void*);
int readTupleData(FILE*, void*);
int readTupleText(FILE*, void*);
int writeTupleData(FILE*, void*);
int writeTupleText(FILE*, void*);

#endif // TUPLE_H_INCLUDED

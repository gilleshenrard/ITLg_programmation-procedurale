#include "../lib/global.h"

int newTuple(t_tuple *newtuple){
    printf("\n-------------------------------------");
    printf("\nSaisissez l'id : ");
    fflush(stdin);
    scanf("%d", &newtuple->id);
    printf("\nSaisissez le nom de famille : ");
    fflush(stdin);
    scanf("%s", newtuple->lastname);
    printf("\nSaisissez le prenom : ");
    fflush(stdin);
    scanf("%s", newtuple->firstname);
    printf("\nSaisissez la ville : ");
    fflush(stdin);
    scanf("%s", newtuple->city);
    strcpy(newtuple->filler, "");

    return 0;
}

int compareLastName(void* first, void* second){
    t_tuple *first_tuple = (t_tuple*) first;
    t_tuple *second_tuple = (t_tuple*) second;

    return stricmp(first_tuple->lastname, second_tuple->lastname);
}

int compareFilterLastName(void* first, void* second){
    t_tuple *first_tuple = (t_tuple*) first;
    t_tuple *second_tuple = (t_tuple*) second;
    char tmp[28] = "0";

    memcpy(tmp, first_tuple->lastname, strlen(second_tuple->lastname));
    tmp[strlen(second_tuple->lastname)]='\0';

    return stricmp(tmp, second_tuple->lastname);
}

int compareID(void* first, void* second){
    t_tuple *first_tuple = (t_tuple*) first;
    t_tuple *second_tuple = (t_tuple*) second;

    return first_tuple->id - second_tuple->id;
}

int swapTuples(void* first, void* second){
    t_tuple *first_tuple = (t_tuple*)first;
    t_tuple *second_tuple = (t_tuple*)second;
    t_tuple tmp;

    tmp = *first_tuple;
    *first_tuple = *second_tuple;
    *second_tuple = tmp;

    return 0;
}

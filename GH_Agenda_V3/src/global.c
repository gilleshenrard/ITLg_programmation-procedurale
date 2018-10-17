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

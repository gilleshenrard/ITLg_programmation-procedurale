#include "../lib/global.h"

/************************************************************/
/*  I : /                                                   */
/*  P : Displays a tuple (inline)                           */
/*  O : /                                                   */
/************************************************************/
void displayTuple(t_tuple* toDisplay){
    printf("\n%6d\t\t%28s\t\t%32s\t\t%32s\t\t%28s", toDisplay->id, toDisplay->lastname, toDisplay->firstname, toDisplay->city, toDisplay->job);
}

/************************************************************/
/*  I : Tuple to encode                                     */
/*  P : Allows the user to enter a tuple fields values      */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
int encodeTuple(t_tuple *encodeTuple){
    printf("\n\n-------------------------------------\n");

    printf("Saisissez l'id : ");
    fflush(stdin);
    scanf("%d", &encodeTuple->id);

    printf("Saisissez le nom de famille : ");
    fflush(stdin);
    scanf("%s", encodeTuple->lastname);

    printf("Saisissez le prenom : ");
    fflush(stdin);
    scanf("%s", encodeTuple->firstname);

    printf("Saisissez la ville : ");
    fflush(stdin);
    scanf("%s", encodeTuple->city);

    printf("Saisissez le metier : ");
    fflush(stdin);
    scanf("%s", encodeTuple->job);

    return 0;
}

/************************************************************/
/*  I : First last name to compare                          */
/*      Second last name to compare                         */
/*  P : Compares (strictly, case insensitive) two last names*/
/*  O : 1 -> First > Second                                 */
/*      0 -> First = Second                                 */
/*     -1 -> First < Second                                 */
/************************************************************/
int compareLastName(void* first, void* second){
    //convert from void* to t_tuple
    t_tuple *first_tuple = (t_tuple*) first;
    t_tuple *second_tuple = (t_tuple*) second;

    return stricmp(first_tuple->lastname, second_tuple->lastname);
}

/************************************************************/
/*  I : First first name to compare                         */
/*      Second first name to compare                        */
/*  P : Compares (strictly, case insensitive) two 1st names */
/*  O : 1 -> First > Second                                 */
/*      0 -> First = Second                                 */
/*     -1 -> First < Second                                 */
/************************************************************/
int compareFirstName(void* first, void* second){
    //convert from void* to t_tuple
    t_tuple *first_tuple = (t_tuple*) first;
    t_tuple *second_tuple = (t_tuple*) second;

    return stricmp(first_tuple->firstname, second_tuple->firstname);
}

/************************************************************/
/*  I : First city name to compare                          */
/*      Second city name to compare                         */
/*  P : Compares (strictly, case insensitive) two cities    */
/*  O : 1 -> First > Second                                 */
/*      0 -> First = Second                                 */
/*     -1 -> First < Second                                 */
/************************************************************/
int compareCity(void* first, void* second){
    //convert from void* to t_tuple
    t_tuple *first_tuple = (t_tuple*) first;
    t_tuple *second_tuple = (t_tuple*) second;

    return stricmp(first_tuple->city, second_tuple->city);
}

/************************************************************/
/*  I : First job to compare                                */
/*      Second job to compare                               */
/*  P : Compares (strictly, case insensitive) two jobs      */
/*  O : 1 -> First > Second                                 */
/*      0 -> First = Second                                 */
/*     -1 -> First < Second                                 */
/************************************************************/
int compareJob(void* first, void* second){
    //convert from void* to t_tuple
    t_tuple *first_tuple = (t_tuple*) first;
    t_tuple *second_tuple = (t_tuple*) second;

    return stricmp(first_tuple->job, second_tuple->job);
}

/************************************************************/
/*  I : First last name to compare                          */
/*      Second last name to compare                         */
/*  P : Compares (loosely, case insensitive) two last names */
/*  O : 1 -> First > Second                                 */
/*      0 -> First = Second                                 */
/*     -1 -> First < Second                                 */
/************************************************************/
int compareFilterLastName(void* first, void* second){
    //convert from void* to t_tuple
    t_tuple *first_tuple = (t_tuple*) first;
    t_tuple *second_tuple = (t_tuple*) second;
    char tmp[28] = "0";

    //crop the last name to compare to size, and comparison
    memcpy(tmp, first_tuple->lastname, strlen(second_tuple->lastname));
    //append \0
    tmp[strlen(second_tuple->lastname)]='\0';

    return stricmp(tmp, second_tuple->lastname);
}

/************************************************************/
/*  I : First ID to compare                                 */
/*      Second ID to compare                                */
/*  P : Compares (strictly) two IDs                         */
/*  O : 1 -> First > Second                                 */
/*      0 -> First = Second                                 */
/*     -1 -> First < Second                                 */
/************************************************************/
int compareID(void* first, void* second){
    //convert from void* to t_tuple
    t_tuple *first_tuple = (t_tuple*) first;
    t_tuple *second_tuple = (t_tuple*) second;

    return first_tuple->id - second_tuple->id;
}

/************************************************************/
/*  I : Tuple to swap                                       */
/*  P : Swaps two tuples                                    */
/*  O : /                                                   */
/************************************************************/
int swapTuples(void* first, void* second){
    //convert from void* to t_tuple
    t_tuple *first_tuple = (t_tuple*)first;
    t_tuple *second_tuple = (t_tuple*)second;
    t_tuple tmp;

    tmp = *first_tuple;
    *first_tuple = *second_tuple;
    *second_tuple = tmp;

    return 0;
}

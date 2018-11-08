#include "../lib/tuple.h"

/************************************************************/
/*  I : Element to display                                  */
/*      Eventual parameter                                  */
/*  P : Displays a tuple (inline)                           */
/*  O : 0 -> Element displayed                              */
/*     -1 -> Error                                          */
/************************************************************/
int displayTupleInline(void* toDisplay, void* parameter){
    t_tuple* display = (t_tuple*)toDisplay;

    if(!toDisplay)
        return -1;

    printf("\n%28s\t\t%32s\t\t%64s", display->lastname, display->firstname, display->email);
    return 0;
}

/************************************************************/
/*  I : Element to display                                  */
/*      Eventual parameter                                  */
/*  P : Displays a tuple with debug info                    */
/*  O : 0 -> Element displayed                              */
/*     -1 -> Error                                          */
/************************************************************/
int displayTupleDebug(void* toDisplay, void* parameter){
    t_tuple* display = (t_tuple*)toDisplay;

    if(!toDisplay)
        return -1;

    printf("\n%10p\t%10p\t%32s\t%10p", display->previous, display, display->lastname, display->next);
    return 0;
}

/************************************************************/
/*  I : Element to display                                  */
/*      Eventual parameter                                  */
/*  P : Displays a tuple (block)                            */
/*  O : 0 -> Element displayed                              */
/*     -1 -> Error                                          */
/************************************************************/
int displayTupleBlock(void* toDisplay, void* parameter){
    t_tuple* display = (t_tuple*)toDisplay;

    if(!toDisplay)
        return -1;

    printf("\n################################################################");
    printf("\n#%64d#", display->id);
    printf("\n#%64s#", display->firstname);
    printf("\n#%64s#", display->lastname);
    printf("\n#%64s#", display->birthdate);
    printf("\n#%64s#", display->phone);
    printf("\n#%64s#", display->mobile);
    printf("\n#%64s#", display->email);
    printf("\n#%64s#", display->postcode);
    printf("\n#%64s#", display->city);
    printf("\n#%64s#", display->update);
    printf("\n################################################################");

    return 0;
}

/************************************************************/
/*  I : Tuple to encode                                     */
/*  P : Allows the user to enter a tuple fields values      */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
int encodeTuple(t_tuple *encodeTuple){

    if(!encodeTuple)
        return -1;

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

    printf("Saisissez la date de naissance : ");
    fflush(stdin);
    scanf("%s", encodeTuple->birthdate);

    printf("Saisissez le numero de telephone : ");
    fflush(stdin);
    scanf("%s", encodeTuple->phone);

    printf("Saisissez le numero de portable : ");
    fflush(stdin);
    scanf("%s", encodeTuple->mobile);

    printf("Saisissez l'adresse email : ");
    fflush(stdin);
    scanf("%s", encodeTuple->email);

    printf("Saisissez le code postal : ");
    fflush(stdin);
    scanf("%s", encodeTuple->postcode);

    printf("Saisissez la ville : ");
    fflush(stdin);
    scanf("%s", encodeTuple->city);

    printf("Saisissez la date de mise a jour : ");
    fflush(stdin);
    scanf("%s", encodeTuple->update);

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
/*  O : >0 -> First > Second                                 */
/*       0 -> First = Second                                 */
/*      <0 -> First < Second                                 */
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
/*  O : 0 -> Swapped                                        */
/*     -1 -> Error                                          */
/************************************************************/
int swapTuples(void* first, void* second){
    t_tuple tmp;

    if(!first || !second)
        return -1;

    memset(&tmp, 0, sizeof(t_tuple));

    assignTuples((void*)&tmp, first);
    assignTuples(first, second);
    assignTuples(second, (void*)&tmp);

    return 0;
}

/************************************************************/
/*  I : Tuple to assign values to                           */
/*      Tuple to assign values from                         */
/*  P : Assigns the values from the new tuple to the old one*/
/*          note : keeps the pointers untouched             */
/*  O : /                                                   */
/************************************************************/
int assignTuples(void* oldelem, void* newelem){
    t_tuple* oldTuple = (t_tuple*)oldelem;
    t_tuple* newTuple = (t_tuple*)newelem;
    t_tuple *saveNext = NULL, *savePrevious=NULL;

    if(!oldelem || !newelem)
        return -1;

    //save the pointer values of the old tuple
    saveNext = oldTuple->next;
    savePrevious = oldTuple->previous;

    //copy the data from the new tuple to the old one
    *oldTuple = *newTuple;

    //restore the pointer values
    oldTuple->next = saveNext;
    oldTuple->previous = savePrevious;

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Gets the next element to the current one            */
/*  O : Address of the next elemnt                          */
/*          (NULL if current is null)                       */
/************************************************************/
void** nextTuple(void* current){
    t_tuple* currentTuple = (t_tuple*)current;

    if(!current)
        return NULL;

    return (void**)&currentTuple->next;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Gets the previous element of the current one        */
/*  O : Address of the previous elemnt                      */
/*          (NULL if current is null)                       */
/************************************************************/
void** previousTuple(void* current){
    t_tuple* currentTuple = (t_tuple*)current;

    if(!current)
        return NULL;

    return (void**)&currentTuple->previous;
}

/************************************************************/
/*  I : Data file in which read the line                    */
/*      Tuple in which copy the read data                   */
/*  P : Reads one line of a data file into a tuple          */
/*  O :  0 -> OK                                            */
/*      -1 -> Error                                         */
/************************************************************/
int readTupleData(FILE* file, void* tuple){
    if(fread(tuple, sizeof(t_tuple), 1, file) < 1)
        return -1;
    else
        return 0;
}

/************************************************************/
/*  I : Text file in which read the line                    */
/*      Tuple in which copy the read data                   */
/*  P : Reads one line of a text file into a tuple          */
/*  O :  0 -> OK                                            */
/*      -1 -> Error                                         */
/************************************************************/
int readTupleText(FILE* file, void* elem){
    t_tuple* tmp = (t_tuple*)elem;

    memset(tmp, 0, sizeof(t_tuple));
    if(fscanf(file, "%d %s %s %s %s %s %s %s %s %s\n", &tmp->id, tmp->firstname, tmp->lastname, tmp->birthdate, tmp->phone, tmp->mobile, tmp->email, tmp->postcode, tmp->city, tmp->update) < 10)
        return -1;
    else
        return 0;
}

/************************************************************/
/*  I : Data file in which write the line                   */
/*      Tuple to write                                      */
/*  P : Writes a tuple in a file                            */
/*  O :  0 -> OK                                            */
/*      -1 -> Error                                         */
/************************************************************/
int writeTupleData(FILE* file, void* tuple){
    if(fwrite(tuple, sizeof(t_tuple), 1, file) < 1)
        return -1;
    else
        return 0;
}

/************************************************************/
/*  I : Text file in which write the line                   */
/*      Tuple to write                                      */
/*  P : Writes a tuple in a file                            */
/*  O :  0 -> OK                                            */
/*      -1 -> Error                                         */
/************************************************************/
int writeTupleText(FILE* file, void* elem){
    t_tuple* tmp = (t_tuple*)elem;

    if(fprintf(file, "%d %s %s %s %s %s %s %s %s %s\n", tmp->id, tmp->firstname, tmp->lastname, tmp->birthdate, tmp->phone, tmp->mobile, tmp->email, tmp->postcode, tmp->city, tmp->update) < 10)
        return -1;
    else
        return 0;
}

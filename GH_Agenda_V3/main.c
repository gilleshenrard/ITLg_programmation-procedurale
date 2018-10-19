#include "lib/global.h"
#include "lib/inout.h"
#include "lib/algo.h"
#include "lib/tuple.h"

int menuAppendFile();
int searchList(e_criteria);
int sortTab(FILE*, t_list_meta*, e_criteria, t_tuple**);
int searchIndex();
int listFile();

int main(int argc, char *argv[])
{
    char choice=0;
    char princ_menu[7][32]={"Main menu",
                            "Creer des enregistrements",
                            "Chercher un nom",
                            "Chercher un numero de sequence",
                            "Lister tous les enregistrements",
                            "Changer d'agenda",
                            "Quitter"};

    do{
        choice = menu(sizeof(princ_menu)/32, princ_menu);
        switch(choice){
            case '0':   //Record creation and addition at the end of the file
                menuAppendFile();
                break;

            case '1':   //Research for a last name (possibly incomplete)
                searchList(LASTNAME);
                break;

            case '2':   //Research a record by its index in the file
                searchIndex();
                break;

            case '3':   //List all the records of a file
                listFile();
                break;

            case '4':
                P_SEP
                printf("\nSaisissez le nom du fichier : ");
                fflush(stdin);
                scanf("%s", filename);
                break;

            default:
                break;
        }
    }while(choice!=27);

    return EXIT_SUCCESS;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Subroutine to append a new tuple to the file        */
/*  O : Choice                                              */
/************************************************************/
int menuAppendFile(){
    t_tuple current;
    FILE *file=NULL;

    memset(&current, 0, sizeof(t_tuple));
    encodeTuple(&current);
    //Opening or creation of the file
    if((file = fopen(filename, "r+b")) == NULL)
        file = fopen(filename, "w+b");
    appendFile(file, &current);
    fclose(file);

    return 0;
}

/************************************************************/
/*  I : File to manipulate                                  */
/*      Criteria of the research (e.g. field to research)   */
/*      Number of records in the file                       */
/*  P : Searches in the file for the given value            */
/*  O : 1 -> Records found                                  */
/*      0 -> None found                                     */
/*     -1 -> Error                                          */
/************************************************************/
int searchList(e_criteria criteria){
    FILE* file=NULL;
    int nbrecords;
    t_tuple *first=NULL;
    t_algo_meta meta = {NULL, 0, sizeof(t_tuple), NULL, &swapTuples};
    t_list_meta metalist = {meta, &assignTuples, &nextTuple};

    //Open the file
    file = fopen(filename, "rb");
    if(file){
        //Retrieve the number of records in the file
        fseek(file, 0, SEEK_END);
        nbrecords = ftell(file)/sizeof(t_tuple);
    }

    //copy the matching search elements from the file to a list via a tab
    metalist.meta.tabsize = nbrecords;
    sortTab(file, &metalist, criteria, &first);
    fclose(file);

    //Display all the records found, then deallocate the list
    foreachList(&metalist, (void**)&first, NULL, &displayTuple);
    foreachList(&metalist, (void**)&first, NULL, &freeTuple);

    //Wait for a user input
    fflush(stdin);
    getch();

    return 1;
}

/************************************************************/
/*  I : File to manipulate                                  */
/*      Metadata necessary to the algorithms                */
/*      Criteria of the research (e.g. field to research)   */
/*      First element of the list                           */
/*  P : Extracts all the elements from the file, copies it  */
/*          in a tab and sorts it, then copies all matching */
/*          elements in a dynamic list                      */
/*  O : /                                                   */
/************************************************************/
/*  THIS FUNCTION HAS BEEN CREATED FOR THE SOLE PURPOSE OF  */
/*  AVOIDING KEEPING THE TAB IN MEMORY FOR TOO LONG         */
/************************************************************/
int sortTab(FILE* file, t_list_meta *metalist, e_criteria criteria, t_tuple **first){
    int search = 0, nbrecords = metalist->meta.tabsize;
    t_tuple tab[nbrecords], tmp, *last=NULL;
    char name[28]="0";

    //Sequentially read the full file and add its content in a buffer array
    fseek(file, 0, SEEK_SET);
    for(int i=0 ; i<nbrecords ; i++)
        if(fread(&tab[i], sizeof(t_tuple), 1, file) < 1){
            fprintf(stderr, "\nsearchList : Erreur pendant la lecture du fichier");
            fflush(stdin);
            getch();
            return 0;
        }

    metalist->meta.tab = (void*)&tab;

    //Sort the array according to the provided criteria
    switch(criteria){
        case LASTNAME:
            metalist->meta.doCompare = &compareFilterLastName;
            break;

        case ID:
            metalist->meta.doCompare = &compareID;
            break;

        case FIRSTNAME:
            metalist->meta.doCompare = &compareFirstName;
            break;

        case CITY:
            metalist->meta.doCompare = &compareCity;
            break;

        case JOB:
            metalist->meta.doCompare = &compareJob;
            break;

        default:
            fprintf(stderr, "\nsearchList : Mauvais critere envoye");
            return -1;
            break;
    }
    quickSort(&metalist->meta, 0, nbrecords-1);

    //Request for the string to find in the records
    P_SEP
    printf("\nSaisissez le nom de famille a chercher : ");
    fflush(stdin);
    scanf("%s", name);

    //Bufferise the string in a tuple and binary search
    strcpy(tmp.lastname, name);
    search = binarySearchFirst(&metalist->meta, (void*)&tmp);
    if(search <0){
        fprintf(stderr, "\nsearchList : Nom '%s' non-trouve...", name);
        fflush(stdin);
        getch();
        return 0;
    }

    //Append all the strings compatible with the criteria in a chained list
    while((*metalist->meta.doCompare)((void*)&tab[search], (void*)&tmp) <= 0 && search < nbrecords){
        appendUnsortedList(metalist, (void**)first, (void**)&last, (void*)&tab[search]);
        search++;
    }

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Searches in the file according to its index         */
/*          in the file                                     */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
int searchIndex(){
    FILE* file=NULL;
    int index = 0, nbrecords=0;;
    t_tuple record;

    file = fopen(filename, "rb");
    if(file){
        //Retrieve the number of records in the file
        fseek(file, 0, SEEK_END);
        nbrecords = ftell(file)/sizeof(t_tuple);
        fseek(file, 0, SEEK_SET);
    }

    if(nbrecords <= 0){
        fprintf(stderr, "\nsearchIndex : Fichier vide...");

        //Wait for a user input
        fflush(stdin);
        getch();
        return -1;
    }

    //Request for the index of the record to find
    P_SEP
    printf("\nSaisissez l'index de l'enregistrement : ");
    fflush(stdin);
    scanf("%d", &index);

    if(index >= nbrecords){
        fprintf(stderr, "\nsearchIndex : Index out of range...");

        //Wait for a user input
        fflush(stdin);
        getch();
        return -1;
    }

    //jump to the index specified and read the record
    fseek(file, index*sizeof(t_tuple), SEEK_SET);
    if(fread(&record, sizeof(t_tuple), 1, file) < 1){
            fprintf(stderr, "\nsearchIndex : Erreur pendant la lecture du fichier");
            return -1;
        }

    //display the record
    displayTuple((void*)&record, NULL);
    fflush(stdin);
    getch();

    fclose(file);
    return 0;
}

/************************************************************/
/*  I : File to manipulate                                  */
/*      Number of records in the file                       */
/*  P : Lists all the records of a file                     */
/*  O : /                                                   */
/************************************************************/
int listFile(){
    FILE* file=NULL;
    int nbrecords=0;
    t_tuple record;

    file = fopen(filename, "rb");
    if(file){
        //Retrieve the number of records in the file
        fseek(file, 0, SEEK_END);
        nbrecords = ftell(file)/sizeof(t_tuple);
        fseek(file, 0, SEEK_SET);
    }

    P_SEP
    for (int i=0 ; i<nbrecords ; i++){
        fread(&record, sizeof(t_tuple), 1, file);
        displayTuple((void*)&record, NULL);
    }
    fflush(stdin);
    getch();

    fclose(file);
    return 0;
}

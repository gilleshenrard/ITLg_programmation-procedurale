#include "lib/global.h"
#include "lib/inout.h"
#include "lib/algo.h"
#include "lib/tuple.h"

char menu(int, char[][32]);
int menuAppendFile();
int menuSearchList(e_criteria);
int sortTab(FILE*, t_algo_meta*, e_criteria, t_tuple**);
int menuSearchIndex();
int menuListFile();

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
                menuSearchList(LASTNAME);
                break;

            case '2':   //Research a record by its index in the file
                menuSearchIndex();
                break;

            case '3':   //List all the records of a file
                menuListFile();
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
/*  I : Number of possible sections in the menu             */
/*      Sections of the menu to display                     */
/*  P : Displays the menu and returns the user's choice     */
/*  O : Choice                                              */
/************************************************************/
char menu(int i, char sections[i][32]){
    int j;
    char choice = 0;

    system("cls");

    printf("****************************************\n");
    printf("\t\t%s\n", sections[0]);
    printf("****************************************\n");
    printf("Current file : %s\n\n", filename);
    for(j=1; j<i-1; j++)
        printf("%d) %s\n", j-1, sections[j]);
    printf("ESC) %s\n", sections[j]);

    printf("\nEffectuez votre choix : ");
    fflush(stdin);
    choice = getch();

    return choice;
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
    if(openFile(&file, "r+") < 0)
        openFile(&file, "w+");
    appendFile(file, (void*)&current, sizeof(t_tuple));
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
int menuSearchList(e_criteria criteria){
    FILE* file=NULL;
    int nbrecords;
    t_tuple *first=NULL;
    t_algo_meta meta = {NULL, 0, sizeof(t_tuple), NULL, &swapTuples, &assignTuples, &nextTuple};

    //Open the file
    openFile(&file, "r");
    if(file){
        //Retrieve the number of records in the file
        fseek(file, 0, SEEK_END);
        nbrecords = ftell(file)/sizeof(t_tuple);
    }

    //copy the matching search elements from the file to a list via a tab
    meta.nbelements = nbrecords;
    sortTab(file, &meta, criteria, &first);
    fclose(file);

    //Display all the records found, then deallocate the list
    foreachList(&meta, (void**)&first, NULL, &displayTupleInline);
    foreachList(&meta, (void**)&first, NULL, &freeTuple);

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
int sortTab(FILE* file, t_algo_meta *meta, e_criteria criteria, t_tuple **first){
    int search = 0, nbrecords = meta->nbelements;
    t_tuple tab[nbrecords], tmp;
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

    meta->structure = (void*)&tab;

    //Sort the array according to the provided criteria
    switch(criteria){
        case LASTNAME:
            meta->doCompare = &compareFilterLastName;
            break;

        default:
            fprintf(stderr, "\nsearchList : Mauvais critere envoye");
            return -1;
            break;
    }
    quickSort(meta, 0, nbrecords-1);

    //Request for the string to find in the records
    P_SEP
    printf("\nSaisissez le nom de famille a chercher : ");
    fflush(stdin);
    scanf("%s", name);

    //Bufferise the string in a tuple and binary search
    strcpy(tmp.lastname, name);
    search = binarySearchFirst(meta, (void*)&tmp);
    if(search <0){
        fprintf(stderr, "\nsearchList : Nom '%s' non-trouve...", name);
        fflush(stdin);
        getch();
        return 0;
    }

    //Append all the strings compatible with the criteria in a chained list
    while((*meta->doCompare)((void*)&tab[search], (void*)&tmp) <= 0 && search < nbrecords){
        insertListTop(meta, (void**)first, (void*)&tab[search]);
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
int menuSearchIndex(){
    FILE* file=NULL;
    int index = 0, nbrecords=0;;
    t_tuple record;

    openFile(&file, "r");
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
    displayTupleBlock((void*)&record, NULL);
    fflush(stdin);
    getch();

    fclose(file);
    return 0;
}

/************************************************************/
/*  I : File to manipulate                                  */
/*      Number of records in the file                       */
/*  P : Lists all the records of a file                     */
/*  O :  0 -> OK                                            */
/*      -1 -> Error                                         */
/************************************************************/
int menuListFile(){
    FILE* file=NULL;
    t_tuple record;
    int (*doRead)(FILE*, void*);

    openFile(&file, "r");
    if(file){
        doRead = (isTextFile() ? &readTextLine : &readDataLine);

        while(!(*doRead)(file, (void*)&record))
            displayTupleInline((void*)&record, NULL);

        fflush(stdin);
        getch();

        fclose(file);
        return 0;
    }
    else
        return -1;
}

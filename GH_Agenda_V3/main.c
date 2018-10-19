#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include "lib/global.h"
#include "lib/inout.h"
#include "lib/algo.h"
#include "lib/tuple.h"

#define P_SEP printf("\n---------------------------------------------------------------------------------------------------------------\n");

char filename[] = "test.dat";

char menu(int, char[][32]);
int searchList(FILE*, e_criteria, int);
int searchIndex(FILE*, int);
int listFile(FILE*, int);

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    int filesize = 0;
    char choice=0;
    t_tuple current;
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
                memset(&current, 0, sizeof(t_tuple));
                encodeTuple(&current);
                //Opening or creation of the file
                if((file = fopen(filename, "r+b")) == NULL)
                    file = fopen(filename, "w+b");
                appendFile(file, &current);
                fclose(file);
                break;

            case '1':   //Research for a last name (possibly incomplete)
                //Open the file
                file = fopen(filename, "rb");
                if(file){
                    //Retrieve the number of records in the file
                    fseek(file, 0, SEEK_END);
                    filesize = ftell(file)/sizeof(t_tuple);
                    fseek(file, 0, SEEK_SET);
                    //Call the research function
                    searchList(file, LASTNAME, filesize);
                    fclose(file);
                }
                break;

            case '2':   //Research a record by its index in the file
                //Open the file
                file = fopen(filename, "rb");
                if(file){
                    //Retrieve the number of records in the file
                    fseek(file, 0, SEEK_END);
                    filesize = ftell(file)/sizeof(t_tuple);
                    fseek(file, 0, SEEK_SET);
                    //Call the research function
                    searchIndex(file, filesize);
                    fclose(file);
                }
                break;

            case '3':   //List all the records of a file
                //Open the file
                file = fopen(filename, "rb");
                if(file){
                    //Retrieve the number of records in the file
                    fseek(file, 0, SEEK_END);
                    filesize = ftell(file)/sizeof(t_tuple);
                    fseek(file, 0, SEEK_SET);
                    listFile(file, filesize);
                    fclose(file);
                }
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
/*  I : File to manipulate                                  */
/*      Criteria of the research (e.g. field to research)   */
/*      Number of records in the file                       */
/*  P : Searches in the file for the given value            */
/*  O : 1 -> Records found                                  */
/*      0 -> None found                                     */
/*     -1 -> Error                                          */
/************************************************************/
int searchList(FILE* file, e_criteria criteria, int nbrecords){
    t_tuple *first=NULL, *last=NULL, *cur=NULL, *prev=NULL, tmp;
    t_tuple tab[nbrecords];
    char name[28]="0";
    int search = 0;
    t_algo_meta meta = {NULL, nbrecords, sizeof(t_tuple), NULL, &swapTuples, &assignTuples};

    //Sequentially read of the full file and add its content in a buffer array
    for(int i=0 ; i<nbrecords ; i++)
        if(fread(&tab[i], sizeof(t_tuple), 1, file) < 1){
            fprintf(stderr, "\nsearchList : Erreur pendant la lecture du fichier");
            return -1;
        }

    meta.tab = (void*)&tab;

    //Sort the array according to the provided criteria
    switch(criteria){
        case LASTNAME:
            //bubbleSort((void*)tab, nbrecords, sizeof(t_tuple), &compareLastName, &swapTuples);
            meta.doCompare = &compareLastName;
            break;

        case ID:
            //bubbleSort((void*)tab, nbrecords, sizeof(t_tuple), &compareID, &swapTuples);
            meta.doCompare = &compareID;
            break;

        case FIRSTNAME:
            meta.doCompare = &compareFirstName;
            break;

        case CITY:
            meta.doCompare = &compareCity;
            break;

        case JOB:
            meta.doCompare = &compareJob;
            break;

        default:
            fprintf(stderr, "\nsearchList : Mauvais critere envoye");
            return -1;
            break;
    }
    quickSort(&meta, 0, nbrecords-1);

    //Request for the string to find in the records
    P_SEP
    printf("\nSaisissez le nom de famille a chercher : ");
    fflush(stdin);
    scanf("%s", name);

    //Bufferise the string in a tuple and binary search
    strcpy(tmp.lastname, name);
    meta.doCompare = compareFilterLastName;
    search = binarySearchFirst(&meta, (void*)&tmp);
    if(search <0){
        fprintf(stderr, "\nsearchList : Nom '%s' non-trouve...", name);

        //Wait for a user input
        fflush(stdin);
        getch();
        return 0;
    }

    //Append all the strings compatible with the criteria in a chained list
    while(compareFilterLastName((void*)&tab[search], (void*)&tmp) <= 0 && search < nbrecords){
        appendUnsortedList(&meta, (void**)&first, (void**)&last, (void*)&tab[search], &nextTuple);
        search++;
    }

    //Display all the records found
    cur = first;
    do{
        displayTuple(cur);
        cur = cur->next;
    }while(cur);

    //Memory deallocation of the list
    cur = first;
    do{
        prev = cur;
        cur = cur->next;
        free(prev);
    }while(cur);

    //Wait for a user input
    fflush(stdin);
    getch();

    return 1;
}

/************************************************************/
/*  I : File to manipulate                                  */
/*      Number of records in the file                       */
/*  P : Searches in the file according to its index         */
/*          in the file                                     */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
int searchIndex(FILE* file, int nbrecords){
    int index = 0;
    t_tuple record;

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
    displayTuple(&record);
    fflush(stdin);
    getch();

    return 0;
}

/************************************************************/
/*  I : File to manipulate                                  */
/*      Number of records in the file                       */
/*  P : Lists all the records of a file                     */
/*  O : /                                                   */
/************************************************************/
int listFile(FILE* file, int nbrecords){
    t_tuple record;

    P_SEP
    for (int i=0 ; i<nbrecords ; i++){
        fread(&record, sizeof(t_tuple), 1, file);
        displayTuple(&record);
    }

    //display the record
    displayTuple(&record);
    fflush(stdin);
    getch();

    return 0;
}

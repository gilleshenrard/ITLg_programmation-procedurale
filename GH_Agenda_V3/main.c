#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include "lib/global.h"
#include "lib/inout.h"
#include "lib/structure.h"
#include "lib/algo.h"

char filename[] = "test.dat";

char menu(int, char[][32]);
int searchList(FILE*, e_criteria, int);

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    int filesize = 0;
    char choice=0;
    t_tuple current;
    char princ_menu[5][32]={"Main menu",
                            "Creer des enregistrements",
                            "Chercher un nom",
                            "Chercher un numero de sequence",
                            "Quitter"};

    do{
        choice = menu(5, princ_menu);
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

    printf("***********************************\n");
    printf("%s\n", sections[0]);
    printf("***********************************\n");
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
    t_tuple* first=NULL;
    t_tuple* last=NULL;
    t_tuple tab[nbrecords];
    t_tuple tmp;
    t_tuple *cur=NULL, *prev=NULL;
    char name[28]="0";
    int search = 0;

    //Sequentially read of the full file and add its content in a buffer array
    for(int i=0 ; i<nbrecords ; i++)
        if(fread(&tab[i], sizeof(t_tuple), 1, file) < 1){
            fprintf(stderr, "\nErreur pendant la lecture du fichier");
            return -1;
        }

    //Sort the array according to the provided criteria
    switch(criteria){
        case LASTNAME:
            bubbleSort((void*)tab, nbrecords, sizeof(t_tuple), &compareLastName, &swapTuples);
            break;

        case ID:
            bubbleSort((void*)tab, nbrecords, sizeof(t_tuple), &compareID, &swapTuples);
            break;

        default:
            break;
    }

    //Request for the string to find in the records
    printf("\n-------------------------------------");
    printf("\nSaisissez le nom de famille a chercher : ");
    fflush(stdin);
    scanf("%s", name);

    //Bufferise the string in a tuple and binary search
    strcpy(tmp.lastname, name);
    search = binarySearch((void*)tab, nbrecords, sizeof(t_tuple), &compareFilterLastName, (void*)&tmp);
    if(!search){
        fprintf(stderr, "\nNom '%s' non-trouve...", name);

        //Wait for a user input
        fflush(stdin);
        getch();
        return 0;
    }

    //Append all the strings compatible with the criteria in a chained list
    while(compareFilterLastName((void*)&tab[search], (void*)&tmp) <= 0 && search < nbrecords){
        appendList(&first, &last, &tab[search]);
        search++;
    }

    //Display all the records found
    cur = first;
    do{
        printf("\n%s", cur->lastname);
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

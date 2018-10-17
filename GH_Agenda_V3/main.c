#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include "lib/global.h"
#include "lib/inout.h"
#include "lib/structure.h"
#include "lib/algo.h"

char filename[] = "test.dat";

char menu(int, char[][32]);
int searchList(FILE*, e_critera, int);

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    int filesize = 0;
    char choice=0;
    t_tuple current;
    char princ_menu[4][32]={"Creer des enregistrements",
                            "Chercher un nom",
                            "Chercher un numero de sequence",
                            "Quitter"};

    do{
        choice = menu(4, princ_menu);
        switch(choice){
            case '0':
                memset(&current, 0, sizeof(t_tuple));
                newTuple(&current);
                if((file = fopen(filename, "r+b")) == NULL)
                    file = fopen(filename, "w+b");
                appendFile(file, &current);
                fclose(file);
                break;

            case '1':
                file = fopen(filename, "rb");
                if(file){
                    fseek(file, 0, SEEK_END);
                    filesize = ftell(file)/sizeof(t_tuple);
                    fseek(file, 0, SEEK_SET);
                    searchList(file, LASTNAME, filesize);
                    fclose(file);
                }
                break;

            case '2':
                break;

            default:
                break;
        }
    }while(choice!=27);

    return EXIT_SUCCESS;
}

/************************************************************/
/*  I : Sections of the menu to display                     */
/*  P : Displays the menu and returns the user's choice     */
/*  O : Choice                                              */
/************************************************************/
char menu(int i, char sections[i][32]){
    int j;
    char choice = 0;

    system("cls");

    for(j=0; j<i-1; j++)
        printf("%d) %s\n", j, sections[j]);
    printf("ESC) %s\n", sections[j]);

    printf("Effectuez votre choix");
    fflush(stdin);
    choice = getch();

    return choice;
}

int searchList(FILE* file, e_critera critera, int filesize){
    t_tuple* first=NULL;
    t_tuple* last=NULL;
    t_tuple tab[filesize];
    t_tuple tmp;
    t_tuple *cur=NULL, *prev=NULL;
    char name[28]="0";
    int search = 0;

    for(int i=0 ; i<filesize ; i++)
        if(fread(&tab[i], sizeof(t_tuple), 1, file) < 1)
            return -1;

    switch(critera){
        case LASTNAME:
            bubbleSort((void*)tab, filesize, sizeof(t_tuple), &compareLastName, &swapTuples);
            break;

        case ID:
            bubbleSort((void*)tab, filesize, sizeof(t_tuple), &compareID, &swapTuples);
            break;

        default:
            break;
    }

    printf("\n-------------------------------------");
    printf("\nSaisissez le nom de famille a chercher : ");
    fflush(stdin);
    scanf("%s", name);

    strcpy(tmp.lastname, name);
    search = binarySearch((void*)tab, filesize, sizeof(t_tuple), &compareFilterLastName, (void*)&tmp);
    if(search < 0)
        printf("\nnon-trouve");
    else{
        while(compareFilterLastName((void*)&tab[search], (void*)&tmp) <= 0 && search < filesize){
            appendList(&first, &last, &tab[search]);
            search++;
        }
        cur = first;
        do{
            printf("\n%s", cur->lastname);
            cur = cur->next;
        }while(cur);

        cur = first;
        do{
            prev = cur;
            cur = cur->next;
            free(prev);
        }while(cur);
    }
    fflush(stdin);
    getch();

    return 0;
}

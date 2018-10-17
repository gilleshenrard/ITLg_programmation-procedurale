#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include "lib/global.h"
#include "lib/inout.h"

char filename[] = "test.dat";

char menu(int, char[][32]);

int main(int argc, char *argv[])
{
    FILE *file = NULL;
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
                appendTuple(file, &current);
                fclose(file);
                break;

            case '1':
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

#include "lib/global.h"
#include "lib/inout.h"
#include "lib/algo.h"
#include "lib/tuple.h"

char menu(int, char[][32]);
int menuAppendFile();
int menuSearchList(e_criteria);
//int menuSearchIndex();
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
                //menuSearchIndex();
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
/*  O :  0 -> File appended                                 */
/*      -1 -> Error                                         */
/************************************************************/
int menuAppendFile(){
    t_tuple record;
    FILE *file=NULL;
    int ret = 0;
    int (*doWrite)(FILE*, void*);

    memset(&record, 0, sizeof(t_tuple));
    encodeTuple(&record);
    //Opening or creation of the file
    if(openFile(&file, "a+") < 0)
        openFile(&file, "w+");

    if(file != NULL){
        doWrite = (isTextFile() ? &writeTupleText : &writeTupleData);
        if((*doWrite)(file, (void*)&record) < 0)
            ret = -1;

        fclose(file);
    }
    else
        ret = -1;

    return ret;
}

/************************************************************/
/*  I : Criteria of the research (e.g. field to research)   */
/*  P : Requests a value to the user, reads the file        */
/*          sequentially and display all the results        */
/*  O : 1 -> Records found                                  */
/*      0 -> None found                                     */
/*     -1 -> Error                                          */
/************************************************************/
int menuSearchList(e_criteria criteria){
    FILE* file=NULL;
    char name[28]="0";
    t_tuple *first=NULL, record, tmp;
    int (*doRead)(FILE*, void*);
    t_algo_meta meta = {(void*)first, 0, sizeof(t_tuple), NULL, &swapTuples, &assignTuples, &nextTuple};

    //switch for the right compare function
    switch(criteria){
        case LASTNAME:
            meta.doCompare = &compareFilterLastName;
            break;
        case FIRSTNAME:
            meta.doCompare = &compareFirstName;
            break;
        case ID:
            meta.doCompare = &compareID;
            break;
        default:
            return -1;
            break;
    }

    //Open the file
    openFile(&file, "r");
    if(file){
        doRead = (isTextFile() ? &readTupleText : &readTupleData);

        //Request for the string to find in the records
        P_SEP
        printf("\nSaisissez le nom de famille a chercher : ");
        fflush(stdin);
        scanf("%s", name);
        memcpy(tmp.lastname, name, sizeof(tmp.lastname));

        //read the file line by line, and add relevant elements to a list
        while(!(*doRead)(file, (void*)&record)){
            if((meta.doCompare)((void*)&record, &tmp) == 0)
                insertListTop(&meta,  &record);
        }

        //if counter = 0, file empty
        if(!meta.nbelements){
            fclose(file);
            return 0;
        }

        //display and free the whole list
        foreachList(&meta, NULL, &displayTupleInline);
        foreachList(&meta, NULL, &freeTuple);

        //Wait for a user input
        fflush(stdin);
        getch();

        fclose(file);
        return 1;
    }

    return -1;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Searches in the file according to its index         */
/*          in the file                                     */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************//*
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
*/

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
        doRead = (isTextFile() ? &readTupleText : &readTupleData);

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


#include "lib/global.h"
#include "lib/inout.h"
#include "lib/algo.h"
#include "lib/tuple.h"

char menu(int, char[][32]);
int menuAppendFile();
int menuSearchList();
//int menuSearchIndex();
int menuListFile();
int menuImportFile();

int main(int argc, char *argv[])
{
    char choice=0;
    char princ_menu[8][32]={"Main menu",
                            "Creer des enregistrements",
                            "Chercher un nom",
                            "Chercher un numero de sequence",
                            "Lister tous les enregistrements",
                            "Changer d'agenda",
                            "Importer depuis un fichier",
                            "Quitter"};

    do{
        choice = menu(sizeof(princ_menu)/32, princ_menu);
        switch(choice){
            case '0':   //Record creation and addition at the end of the file
                if(menuAppendFile() < 0)
                    fprintf(stderr, "\nMain menu : could not append a new element in the file");
                else
                    printf("\nElement appended to the file")
                break;

            case '1':   //Research for a last name (possibly incomplete)
                if(menuSearchList() < 0)
                    fprintf(stderr, "\nMain menu : could not research elements");
                break;

            case '2':   //Research a record by its index in the file
                //menuSearchIndex();
                break;

            case '3':   //List all the records of a file
                if(menuListFile() < 0)
                    fprintf(stderr, "\nMain menu : could not list all elements");
                break;

            case '4':
                P_SEP
                printf("\nSaisissez le nom du fichier : ");
                fflush(stdin);
                scanf("%s", filename);
                break;

            case '5':
                if(menuImportFile() < 0)
                    fprintf(stderr, "\nMain menu : could not import the file");
                break;

            default:
                break;
        }
        fflush(stdin);
        getch();
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
    if(openFile(&file, NULL, "a+") < 0)
        openFile(&file, NULL, "w+");

    //verify if the file is opened
    if(file != NULL){
        //write the record at the end
        doWrite = (isTextFile(NULL) ? &writeTupleText : &writeTupleData);
        if((*doWrite)(file, (void*)&record) < 0){
            fprintf(stderr, "\nmenAppendFile : Error while trying to write in the file");
            ret = -1;
        }

        fclose(file);
    }
    else{
        fprintf(stderr, "\nmenAppendFile : Error while trying to open the file");
        ret = -1;
    }

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
int menuSearchList(){
    FILE* file=NULL;
    t_tuple record, tmp;
    int (*doRead)(FILE*, void*);
    t_algo_meta meta = {NULL, 0, sizeof(t_tuple), &compareFilterLastName, &swapTuples, &assignTuples, &nextTuple};

    //Open the file
    openFile(&file, NULL, "r");
    if(file){
        doRead = (isTextFile(NULL) ? &readTupleText : &readTupleData);

        //Request for the string to find in the records
        P_SEP
        printf("\nSaisissez le nom de famille a chercher : ");
        fflush(stdin);
        scanf("%s", tmp.lastname);

        //read the file line by line, and add relevant elements to a list (sorted with full last name)
        while(!(*doRead)(file, (void*)&record)){
            if((meta.doCompare)((void*)&record, &tmp) == 0){
                meta.doCompare = &compareLastName;
                if(insertListSorted(&meta, &record) < 0){
                    fprintf(stderr, "\nmenuSearchList : Error while inserting element to the list");
                    fclose(file);
                    return -1;
                }
                meta.doCompare = &compareFilterLastName;
            }
        }

        //verify if any elements found
        printf("\n%d elements trouves.", meta.nbelements);
        if(!meta.nbelements){
            fclose(file);
            return 0;
        }

        //display and free the whole list
        if(foreachList(&meta, NULL, &displayTupleInline) < 0){
            fprintf(stderr, "\nmenuSearchList : Error while displaying the elements");
            fclose(file);
            return -1;
        }
        if(foreachList(&meta, NULL, &freeTuple)){
            fprintf(stderr, "\nmenuSearchList : Error while freeing the memory");
            fclose(file);
            return -1;
        }

        fclose(file);
        return 1;
    }

    fprintf(stderr, "\nmenuSearchList : Error while trying to open the file");
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

    //open the file
    openFile(&file, NULL, "r");
    if(file){
        doRead = (isTextFile(NULL) ? &readTupleText : &readTupleData);

        //read the file line per line and display each record read
        while(!(*doRead)(file, (void*)&record)){
            if(displayTupleInline((void*)&record, NULL) < 0){
                fprintf(stderr, "\nmenuListFile : Error while displaying an element");
                fclose(file);
                return -1;
            }
        }

        fclose(file);
        return 0;
    }
    else{
        fprintf(stderr, "\nmenuListFile : Error while trying to open the file");
        return -1;
    }
}

/************************************************************/
/*  I : /                                                   */
/*  P : Asks for a file name to import data from, then      */
/*          appends it at the end of the general file       */
/*  O :  0 -> OK                                            */
/*      -1 -> Error                                         */
/************************************************************/
int menuImportFile(){
    FILE *exportfile=NULL, *importfile=NULL;
    char importname[64]="0";
    t_tuple record;
    int ret=0;
    int (*doRead)(FILE*, void*);
    int (*doWrite)(FILE*, void*);

    //request for the name of the file to import
    P_SEP
    printf("\nSaisissez le nom du fichier a importer : ");
    fflush(stdin);
    scanf("%s", importname);

    //open the file to export to (file name NULL -> take the global file)
    openFile(&exportfile, NULL, "a+");
    if(exportfile){
        //open the file to import
        openFile(&importfile, importname, "r");
        if(importfile){
            doRead = (isTextFile(importname) ? &readTupleText : &readTupleData);
            doWrite = (isTextFile(NULL) ? &writeTupleText : &writeTupleData);

            //read the import file line by line, and append each record to the end of the export file
            while(!(*doRead)(importfile, (void*)&record) && ret > -1){
                if((*doWrite)(exportfile, (void*)&record) < 0){
                    fprintf(stderr, "\nmenuImportFile : Error while trying to write a record in the export file");
                    ret = -1;
                }
            }
            fclose(importfile);
        }
        else{
            fprintf(stderr, "\nmenuImportFile : Error while trying to open the export file");
            ret = -1;
        }

        fclose(exportfile);
    }
    else{
        fprintf(stderr, "\nmenuImportFile : Error while trying to open the import file");
        ret = -1;
    }

    return ret;
}

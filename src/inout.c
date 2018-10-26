#include "../lib/inout.h"

char filename[32] = "test.dat";

/************************************************************/
/*  I : Optional file name to test                          */
/*  P : Tells if the file should be opened as a text file   */
/*          if no file name is provided, will test the      */
/*          global file name                                */
/*  O : 0 -> Binary file                                    */
/*      1 -> Text file                                      */
/************************************************************/
int isTextFile(char* toTest){
    char name[32]="0", opt[4]="0";

    sscanf((toTest ? toTest : filename), "%32[^.].%s", name, opt);
    if(!strcmp(opt, "dat"))
        return 0;
    else
        return 1;
}

/************************************************************/
/*  I : File to open                                        */
/*      Option to open the file (without bin specification) */
/*      Optional file name                                  */
/*  P : Opens the file depending of the extension and option*/
/*          if name is not giver, will use global file name */
/*  O :  0 -> OK                                            */
/*      -1 -> Error                                         */
/************************************************************/
int openFile(FILE** file, char* name, char* option){
    char opt[4]="0";

    if(!isTextFile(name))
        sprintf(opt, "%sb", option);

    *file = fopen((name ? name : filename), option);
    if(!file)
        return -1;

    return 0;
}

#include "../lib/inout.h"

/************************************************************/
/*  I : File to open                                        */
/*      Option to open the file (without bin specification) */
/*  P : Opens the file depending of the extension and option*/
/*  O :  0 -> OK                                            */
/*      -1 -> Error                                         */
/************************************************************/
int openFile(FILE** file, char* option){
    char name[32]="0", opt[4]="0";

    sscanf(filename, "%32[^.].%s", name, opt);

    if(!strcmp(opt, "dat"))
        sprintf(opt, "%sb", option);

    *file = fopen(filename, opt);
    if(!*file)
        return -1;

    return 0;
}

/************************************************************/
/*  I : File in which append the tuple                      */
/*      Tuple to append                                     */
/*      Size of a tuple                                     */
/*  P : Writes a tuple at the end of a file                 */
/*  O :  0 -> OK                                            */
/*      -1 -> Error                                         */
/************************************************************/
int appendFile(FILE* file, void* tuple, int tuplesize){
    int ret;

    fseek(file, 0, SEEK_END);
    ret = fwrite(tuple, tuplesize, 1, file);
    if(ret!=1)
        return -1;

    return 0;
}

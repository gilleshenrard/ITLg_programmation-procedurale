#include "screen.h"

/****************************************************************************************/
/*  I : Error message format string (as in printf)                                      */
/*      Arguments to include to the error string                                        */
/*  P : Prints an error message in STDERR                                               */
/*  O : /                                                                               */
/****************************************************************************************/
void print_error(char* msg, ...){
    char final_msg[128] = {0};
    va_list arg;

    va_start(arg, msg);
    vsprintf(final_msg, msg, arg);
    fprintf(stderr, "%s\n", final_msg);

    va_end(arg);
}

#ifndef INOUT_H_INCLUDED
#define INOUT_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include "global.h"

int openFile(FILE**, char*);
int appendFile(FILE*, void*, int);

#endif // INOUT_H_INCLUDED

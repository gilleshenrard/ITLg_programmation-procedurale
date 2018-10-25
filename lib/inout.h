#ifndef INOUT_H_INCLUDED
#define INOUT_H_INCLUDED
#include <stdio.h>
#include <string.h>

extern char filename[32];

int isTextFile(void);
int openFile(FILE**, char*);

#endif // INOUT_H_INCLUDED

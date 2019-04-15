#ifndef DB_FILE_H_INCLUDED
#define DB_FILE_H_INCLUDED
#include "DB_Main.h"

void Create_DB(dbc *db);
int create_index_unbuffered(dbc* db);

#endif // DB_FILE_H_INCLUDED

#ifndef DB_FILE_H_INCLUDED
#define DB_FILE_H_INCLUDED
#include "DB_Main.h"
#include "algo.h"
#include "files_algo.h"

typedef struct{
    long*   block_off;
    long*   root_off;
    int     elem_size;
}t_datablock;

int Create_DB(dbc *db, char filename[]);
long create_index_file(dbc* db, t_algo_meta* meta, long nb, t_datablock* i_block, t_datablock* t_block);

#endif // DB_FILE_H_INCLUDED

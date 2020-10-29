#ifndef DB_FILE_H_INCLUDED
#define DB_FILE_H_INCLUDED
#include "DB_Main.h"
#include "algo.h"

typedef struct{
    uint32_t*   block_off;
    uint32_t*   root_off;
    int     elem_size;
}t_datablock;

int Create_DB(dbc *db, char filename[]);
long create_index_file(dbc* db, meta_t* meta, uint32_t nb, int (*setSlot)(void*, void*), t_datablock* i_block, t_datablock* t_block);

#endif // DB_FILE_H_INCLUDED

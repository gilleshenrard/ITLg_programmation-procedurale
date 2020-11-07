#ifndef DB_FILE_H_INCLUDED
#define DB_FILE_H_INCLUDED
#include "DB_Main.h"
#include "cstructures.h"
#include "screen.h"

typedef struct{
    uint32_t*   block_off;
    uint32_t*   root_off;
    int     elem_size;
    int (*setSlot)(void*, uint32_t*);
    int (*doCopy)(void*, void*);
}t_datablock;

int Create_DB(dbc *db, char filename[]);
long create_index_file(dbc* db, meta_t* meta, uint32_t nb, t_datablock* i_block, t_datablock* t_block);

long index_tree(FILE* fp, long offset_start, long nb, meta_t* meta);
int searchall_index(FILE* fp, long offset_root, void* key, meta_t* index, meta_t* lis);
int searchone_index(FILE* fp, long offset_root, void* key, meta_t* index, void* element, int elem_size);

#endif // DB_FILE_H_INCLUDED

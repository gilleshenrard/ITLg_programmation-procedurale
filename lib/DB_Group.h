#ifndef DB_GROUP_H_INCLUDED
#define DB_GROUP_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"
#include "algo.h"

#define PRT 0
#define BUF_LEN 200

#define CSV_grp_imp "Data_Import/DB_Group.csv"

typedef struct cgrp_recur cgrp_recur;
typedef struct cgrp_file cgrp_file;

struct cgrp_recur{
    cgrp grp;
    int height;
    cgrp_recur *left;
    cgrp_recur *right;
};

typedef struct i_Group_FK{
    char tp_rec[SZ_TYPE];   //record type (I_GRP)
    int cty_id;             //foreign key
    char filler[8];         //filler to get the index size to 32
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_cgrp_FK;

typedef struct i_Group_name{
    char tp_rec[SZ_TYPE];   //record type (I_GRP)
    char nm_grp[SZ_NAME];   //group name
    char filler[16];        //filler to get the index size to 64
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_cgrp_nm;

//database methods
void Import_CSV_Group(dbc *db);
void Export_CSV_Group(dbc *db);
void Load_Group(dbc *db);
void Print_Group(dbc *db);
void Rec_Group(cgrp *rec);

// dynamic structures methods
int compare_group_FK(void* a, void* b);
int compare_group_nm(void* a, void* b);
int compare_group_FK_index(void* a, void* b);
int compare_group_nm_index(void* a, void* b);
int assign_group_index_FK(void* index, void* elem);
int assign_group_index_slot(void* index, uint32_t* offset);
void** group_right(void* current);
int Rec_group_list(void *record, void* nullable);
char* toString_group(void* current);

#endif // DB_GROUP_H_INCLUDED

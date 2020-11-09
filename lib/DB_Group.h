#ifndef DB_GROUP_H_INCLUDED
#define DB_GROUP_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"

#define CSV_grp_imp "Data_Import/DB_Group.csv"
#define CSV_grp_exp "Data_Export/DB_Group.csv"
#define CSV_grp_header "Id;Nm_Grp;Cd_Cty;Id_Cty\n"

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
int CSVDeserialiseGroup(char *line, void *record);
int CSVFormatGroup(void* elem, char* finalLine);
int Rec_Group(void *rec, void *nullable);

// dynamic structures methods
int compare_group_FK(void* a, void* b);
int compare_group_nm(void* a, void* b);
int compare_group_FK_index(void* a, void* b);
int compare_group_nm_index_char(void* a, void* b);
int assign_group_index_FK(void* index, void* elem);
int assign_group_index_nm(void* index, void* elem);
int assign_group_FK_index_slot(void* index, uint32_t* offset);
int assign_group_nm_index_slot(void* index, uint32_t* offset);

#endif // DB_GROUP_H_INCLUDED

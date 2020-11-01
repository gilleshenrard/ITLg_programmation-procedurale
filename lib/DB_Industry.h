#ifndef DB_INDUSTRY_H_INCLUDED
#define DB_INDUSTRY_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"
#include "algo.h"

#define PRT 0
#define BUF_LEN 200

#define CSV_ind_imp "Data_Import/DB_Industry.csv"

typedef struct cind_recur cind_recur;
typedef struct cind_file cind_file;

struct cind_recur{
    cind ind;
    int height;
    cind_recur *left;
    cind_recur *right;
};

typedef struct i_industry_PK{
    char tp_rec[SZ_TYPE];   //record type (I_IND)
    int ind_id;             //primary key
    char filler[8];         //filler to get the index size to 32
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_cind_PK;

//database methods
void Import_CSV_industry(dbc *db);
void Export_CSV_industry(dbc *db);
void Load_industry(dbc *db);
void Print_industry(dbc *db);
void Rec_industry(cind *rec);

// dynamic structures methods
int compare_industry_PK(void* a, void* b);
int compare_industry_PK_index(void* a, void* b);
int assign_industry_index_PK(void* index, void* elem);
int assign_industry_index_slot(void* index, uint32_t* offset);
void** industry_right(void* current);
int Rec_industry_list(void *record, void* nullable);
char* toString_industry(void* current);

#endif // DB_INDUSTRY_H_INCLUDED

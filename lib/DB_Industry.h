#ifndef DB_INDUSTRY_H_INCLUDED
#define DB_INDUSTRY_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"

#define PRT 0
#define BUF_LEN 200

#define CSV_ind_imp "Data_Import/DB_Industry.csv"
#define CSV_ind_exp "Data_Export/DB_Industry.csv"
#define CSV_ind_header "Id;Nm_sec;Nm_Ind\n"

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
int CSVFormatIndustry(void* elem, char* finalLine);
void Load_industry(dbc *db);
int Rec_industry(void *rec, void *nullable);

// dynamic structures methods
int compare_industry_PK(void* a, void* b);
int compare_industry_PK_index(void* a, void* b);
int assign_industry_index_PK(void* index, void* elem);
int assign_industry_index_slot(void* index, uint32_t* offset);
char* toString_industry(void* current);

#endif // DB_INDUSTRY_H_INCLUDED

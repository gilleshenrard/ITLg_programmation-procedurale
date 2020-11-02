/****************************************************************************************
* Dossier 2 : Database "Analyse de donnees clients"
*
* Manipulations de la table company : fichiers non geres globalement
*
* Programmation procedurale 2019 - E.Bosly - Version 0
****************************************************************************************/
#ifndef DB_COMPANY_H_INCLUDED
#define DB_COMPANY_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"
#include "algo.h"

#define PRT 0
#define BUF_LEN 200

#define CSV_cpy_imp "Data_Import/DB_Company.csv"

typedef struct ccpy_recur ccpy_recur;
typedef struct ccpy_file ccpy_file;

typedef struct i_company_Name{
    char tp_rec[SZ_TYPE];   //record type (I_CPY)
    char nm_cpy[SZ_NAME];   //Company name
    char filler[16];        //filler to get the index size to 64
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_ccpy_name;

typedef struct i_company_Group{
    char tp_rec[SZ_TYPE];   //record type (I_CPY)
    int grp_id;             //group id
    char filler[8];        //filler to get the index size to 32
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_ccpy_grp;

//database methods
void Import_CSV_company(dbc *db);
void Export_CSV_company(dbc *db);
void Load_company(dbc *db);
void Print_company(dbc *db);
void Rec_company(ccpy *rec);

// dynamic structures methods
int compare_company_name(void* a, void* b);
int compare_company_grp(void* a, void* b);
int compare_company_index_name(void* a, void* b);
int compare_company_index_char(void* a, void* b);
int assign_company_index_slot(void* index, uint32_t* offset);
int assign_company_index_name(void* index, void* elem);
int Rec_company_list(void *record, void* nullable);
char* toString_company(void* current);

//file structures methods

#endif // DB_COMPANY_H_INCLUDED

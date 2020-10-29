#ifndef DB_CONTACT_H_INCLUDED
#define DB_CONTACT_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"
#include "algo.h"

#define PRT 0
#define BUF_LEN 200

#define CSV_con_imp "Data_Import/DB_Contact.csv"

typedef struct ccon_recur ccon_recur;
typedef struct ccon_file ccon_file;

struct ccon_recur{
    ccon con;
    int height;
    ccon_recur *left;
    ccon_recur *right;
};

typedef struct i_Contact_cpy{
    char tp_rec[SZ_TYPE];   //record type (I_CON)
    int cpy_id;             //foreign key company
    char filler[8];         //filler to get the index size to 32
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_ccon_cpy;

//database methods
void Import_CSV_contact(dbc *db);
void Export_CSV_contact(dbc *db);
void Load_contact(dbc *db);
void Print_contact(dbc *db);
void Rec_contact(ccon *rec);

// dynamic structures methods
int compare_contact_cpy(void* a, void* b);
int compare_contact_cpy_index(void* a, void* b);
int compare_contact_index_int(void* a, void* b);
int assign_contact_index_slot(void* index, void* offset);
void** contact_right(void* current);
int Rec_contact_list(void *record, void* nullable);
char* toString_contact(void* current);

#endif // DB_CONTACT_H_INCLUDED

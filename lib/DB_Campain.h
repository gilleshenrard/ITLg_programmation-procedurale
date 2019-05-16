#ifndef DB_CAMPAIN_H_INCLUDED
#define DB_CAMPAIN_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"
#include "algo.h"

#define PRT 0
#define BUF_LEN 200

#define CSV_cam_imp "Data_Import/DB_Campaign.csv"

typedef struct ccam_recur ccam_recur;
typedef struct ccam_file ccam_file;

struct ccam_recur{
    ccam cam;
    int height;
    ccam_recur *left;
    ccam_recur *right;
};

typedef struct i_campain_PK{
    char tp_rec[SZ_TYPE];   //record type (I_CAM)
    int cam_id;             //primary key
    char filler[8];         //filler to get the index size to 32
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_ccam_PK;

//database methods
void Import_CSV_campain(dbc *db);
void Export_CSV_campain(dbc *db);
void Load_campain(dbc *db);
void Print_campain(dbc *db);
void Rec_campain(ccam *rec);

// dynamic structures methods
void* allocate_campain(void);
int compare_campain_PK(void* a, void* b);
int compare_campain_PK_index(void* a, void* b);
int compare_campain_PK_int(void* a, void* b);
int compare_campain_index_int(void* a, void* b);
int assign_campain(void* oldelem, void* newelem);
int assign_campain_index(void* oldelem, void* newelem);
int assign_campain_index_PK(void* index, void* elem);
int assign_campain_index_slot(void* index, void* offset);
int swap_campain(void* first, void* second);
int swap_campain_index(void* first, void* second);
void** campain_right(void* current);
void** campain_left(void* current);
int Rec_campain_list(void *record, void* nullable);
char* toString_campain(void* current);
int get_campain_height(void* current);
int set_campain_height(void* current, int value);
void* free_campain(void* campain, void* nullable);

#endif // DB_CAMPAIN_H_INCLUDED

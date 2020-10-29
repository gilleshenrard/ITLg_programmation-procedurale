#ifndef DB_CAMPAIGN_H_INCLUDED
#define DB_CAMPAIGN_H_INCLUDED
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

typedef struct i_campaign_PK{
    char tp_rec[SZ_TYPE];   //record type (I_CAM)
    int cam_id;             //primary key
    char filler[8];         //filler to get the index size to 32
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_ccam_PK;

//database methods
void Import_CSV_campaign(dbc *db);
void Export_CSV_campaign(dbc *db);
void Load_campaign(dbc *db);
void Print_campaign(dbc *db);
void Rec_campaign(ccam *rec);

// dynamic structures methods
int compare_campaign_PK(void* a, void* b);
int compare_campaign_PK_index(void* a, void* b);
int compare_campaign_index_int(void* a, void* b);
int assign_campaign_index_slot(void* index, void* offset);
void** campaign_right(void* current);
int Rec_campaign_list(void *record, void* nullable);
char* toString_campaign(void* current);

#endif // DB_CAMPAIGN_H_INCLUDED

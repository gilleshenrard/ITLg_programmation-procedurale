#ifndef DB_CAMPAIGN_H_INCLUDED
#define DB_CAMPAIGN_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"

#define CSV_cam_imp "Data_Import/DB_Campaign.csv"
#define CSV_cam_exp "Data_Export/DB_Campaign.csv"
#define CSV_cam_header "Id;Nm_Cam;Tp_Cam;Dt_Cam;Nm_Lev;Nm_Dep;Nm_Sec;Nm_Zon;Nr_Yr;Cost\n"

typedef struct i_campaign_PK{
    char tp_rec[SZ_TYPE];   //record type (I_CAM)
    int cam_id;             //primary key
    char filler[8];         //filler to get the index size to 32
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_ccam_PK;

//database methods
int CSVDeserialiseCampaign(char *line, void *record);
int CSVFormatCampaign(void* elem, char* finalLine);
int Rec_campaign(void *rec, void* nullable);

// dynamic structures methods
int compare_campaign_PK(void* a, void* b);
int compare_campaign_PK_index(void* a, void* b);
int compare_campaign_index_int(void* a, void* b);
int assign_campaign_index_PK(void* index, void* elem);
int assign_campaign_index_slot(void* index, uint32_t* offset);
char* toString_campaign(void* current);

#endif // DB_CAMPAIGN_H_INCLUDED

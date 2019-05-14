#ifndef DB_JOB_H_INCLUDED
#define DB_JOB_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"
#include "algo.h"

#define PRT 0
#define BUF_LEN 200

#define CSV_job_imp "Data_Import/DB_Job.csv"

typedef struct cjob_recur cjob_recur;
typedef struct cjob_file cjob_file;

struct cjob_recur{
    cjob job;
    int height;
    cjob_recur *left;
    cjob_recur *right;
};

typedef struct i_job_Name{
    char tp_rec[SZ_TYPE];   //record type (I_JOBNM)
    char nm_job[SZ_NAME];   //job name
    char filler[16];        //filler to get the index size to 64
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_cjob_name;

//database methods
void Import_CSV_job(dbc *db);
void Export_CSV_job(dbc *db);
void Load_job(dbc *db);
void Print_job(dbc *db);
void Rec_job(cjob *rec);

// dynamic structures methods
void* allocate_job(void);
int compare_job_name(void* a, void* b);
int compare_job_index_name(void* a, void* b);
int compare_job_name_char(void* a, void* b);
int compare_job_index_char(void* a, void* b);
int assign_job(void* oldelem, void* newelem);
int assign_job_index(void* oldelem, void* newelem);
int assign_job_index_name(void* index, void* elem);
int assign_job_index_slot(void* index, void* offset);
int swap_job(void* first, void* second);
int swap_job_index(void* first, void* second);
void** job_right(void* current);
void** job_left(void* current);
int Rec_job_list(void *record, void* nullable);
char* toString_job(void* current);
int get_job_height(void* current);
int set_job_height(void* current, int value);
void* free_job(void* job, void* nullable);

#endif // DB_JOB_H_INCLUDED

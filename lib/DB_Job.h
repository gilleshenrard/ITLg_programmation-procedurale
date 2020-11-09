#ifndef DB_JOB_H_INCLUDED
#define DB_JOB_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"

#define CSV_job_imp "Data_Import/DB_Job.csv"
#define CSV_job_exp "Data_Export/DB_Job.csv"
#define CSV_job_header "Id;Nm_Lev;Nm_Dep;Nm_Job\n"

typedef struct i_job_Name{
    char tp_rec[SZ_TYPE];   //record type (I_JOBNM)
    char nm_job[SZ_NAME];   //job name
    char filler[16];        //filler to get the index size to 64
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_cjob_name;

//database methods
int CSVDeserialiseJob(char *line, void *record);
int CSVFormatJob(void* elem, char* finalLine);
int Rec_job(void *rec, void *nullable);

// dynamic structures methods
int compare_job_name(void* a, void* b);
int compare_job_index_name(void* a, void* b);
int assign_job_index_name(void* index, void* elem);
int assign_job_index_slot(void* index, uint32_t* offset);

#endif // DB_JOB_H_INCLUDED

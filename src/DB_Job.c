#include "DB_Job.h"

/****************************************************************************************/
/*  I : CSV line to transform to a Job                                                  */
/*      Buffer in which store the deserialised information                              */
/*  P : Transfrom a CSV line to a Job                                                   */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVDeserialiseJob(char *line, void *record){
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	cjob *job = (cjob*)record;

    //read the job ID (ptr1) and the level nomination (ptr2), then set the job ID
    ptr1 = strtok(line,";");
    ptr2 = strtok(NULL,";");
    memset(fld, 0, BUF_LEN);
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    job->id_job = atoi(fld);

    //set the level nomination and read the department
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(job->nm_lev, ptr1, ptr2-ptr1-1);
    job->nm_lev[strlen(ptr1)]='\0';

    //set the department and read job name
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(job->nm_dep, ptr1, ptr2-ptr1-1);
    job->nm_dep[strlen(ptr1)]='\0';

    //set the job name
    ptr1 = ptr2;
    strncpy(job->nm_job, ptr1, strlen(ptr1));
    job->nm_job[strlen(ptr1)]='\0';

	return 0;
}

/****************************************************************************************/
/*  I : Job to format to CSV                                                            */
/*      Buffer in which the final CSV line will be stored                               */
/*  P : Formats a Job to a CSV file line                                                */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVFormatJob(void* elem, char* finalLine){
    cjob* job = (cjob*)elem;

    sprintf(finalLine,"%d;%s;%s;%s\n",
            job->id_job,
            job->nm_lev,
            job->nm_dep,
            job->nm_job);

    return 0;
}

/****************************************************************************************
* Imprime un record Job depuis le buffer
****************************************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Rec_job(void *rec, void *nullable)
{
    cjob *tmp = (cjob*)rec;

    printf("%3d %28s %28s %36s \n",
           tmp->id_job,
           tmp->nm_lev,
           tmp->nm_dep,
           tmp->nm_job );

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/****************************************************************************************/
/*  I : First Job to compare                                                            */
/*      Second  Job to compare                                                          */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_job_name(void* a, void* b){
    cjob *tmp_a = (cjob*)a;
    cjob *tmp_b = (cjob*)b;

    return strcmp(tmp_a->nm_job, tmp_b->nm_job);
}

/****************************************************************************************/
/*  I : First Job to compare                                                            */
/*      Second  Job to compare                                                          */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_job_index_name(void* a, void* b){
    i_cjob_name *tmp_a = (i_cjob_name*)a;
    i_cjob_name *tmp_b = (i_cjob_name*)b;

    return strcmp(tmp_a->nm_job, tmp_b->nm_job);
}

/****************************************************************************************/
/*  I : Job index buffer to which copy data                                             */
/*      Job from which copy data                                                        */
/*  P : Copies all the fields of a Job to a Job index buffer                            */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_job_index_name(void* index, void* elem){
    cjob* element = (cjob*)elem;
    i_cjob_name* i_element = (i_cjob_name*)index;

    if(!index || !element)
        return -1;

    //copy the data from the job to the buffer
    strcpy(i_element->nm_job, element->nm_job);
    strcpy(i_element->tp_rec, "I_JOBNM");

    return 0;
}

/****************************************************************************************/
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_job_index_slot(void* index, uint32_t* offset){
    i_cjob_name* element = (i_cjob_name*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

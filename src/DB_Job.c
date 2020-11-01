#include "DB_Job.h"

/****************************************************************************************
* Chargement du fichier DB_Job.csv dans la database
*
* PRT controle l'affichage de test (a enlever)
*
****************************************************************************************/
void Import_CSV_job(dbc *db)
{
    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	cjob job;
	FILE *fpi, *fp_lg;

	//open DB files
    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    //open Import file
	fpi = fopen(CSV_job_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nJob : importing ...\n");

    //read the first 200 characters in the import file
    fgets(line, 200, fpi);

    //set the DB file pointer to the beginning of the Jobs data block
    fseek(db->fp, db->hdr.off_job, SEEK_SET);

    printf("%lu\n",(unsigned long int)db->hdr.off_job);

    while (fgets(line, 200, fpi) != NULL)
    {
        //clean the buffer up and set the record type to job
        memset(&job, 0, sizeof(cjob));
        strcpy(job.tp_rec, "JOB");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        //read the job ID (ptr1) and the level nomination (ptr2), then set the job ID
        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        fld[strlen(ptr1)]='\0';
        job.id_job = atoi(fld);                    if (PRT) printf("%d\n", job.id_job);

        //set the level nomination and read the department
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(job.nm_lev, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", job.nm_lev);
        job.nm_lev[strlen(ptr1)]='\0';

        //set the department and read job name
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(job.nm_dep, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", job.nm_dep);
        job.nm_dep[strlen(ptr1)]='\0';

        //set the job name
        ptr1 = ptr2;
        strncpy(job.nm_job, ptr1, strlen(ptr1)-1); if (PRT) printf("%s\n", job.nm_job);
        job.nm_job[strlen(ptr1)]='\0';

        //write the final record
        fwrite(&job, 1, sizeof(cjob), db->fp);

        i++;
    }

    //save the amount of jobs imported
    db->nr_job = i;

    fprintf(fp_lg, "Job imported : %lu\n", (unsigned long int)db->nr_job);

    //close all files
    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nJob imported : %lu\n\n", (unsigned long int)db->nr_job);

	return ;
}

/****************************************************************************************
* Exporte table Job dans un fichier .csv
****************************************************************************************/
void Export_CSV_job(dbc *db)
{
    uint64_t i;
	cjob job;
	FILE *fpo, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    printf("\nJob : exporting ...\n");
    fpo = fopen("Data_Export/Exp_Job.csv", "w");
    fprintf(fpo,"Id;Nm_lev;Nm_dep;Nm_job\n");

    fseek(db->fp, db->hdr.off_job, SEEK_SET);

    for (i=0; i<db->nr_job; i++)
    {
        memset(&job, 0, sizeof(cjob));
        fread(&job, 1, sizeof(cjob), db->fp);

        fprintf(fpo,"%d;%s;%s;%s\n",
                job.id_job,
                job.nm_lev,
                job.nm_dep,
                job.nm_job);
    }

    fprintf(fp_lg, "Job exported : %lu\n", (unsigned long int)db->nr_job);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\nJob exported : %lu\n\n", (unsigned long int)db->nr_job);

    return;
}

/****************************************************************************************
* Charge table Job dans le buffer
****************************************************************************************/
void Load_job(dbc *db)
{
    uint64_t i;
	cjob job;
	FILE *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    if(db->job)
        free(db->job);

    db->job = (cjob*)calloc(db->nr_job, sizeof(cjob));

    printf("\nJob : loading ...\n");

    fseek(db->fp, db->hdr.off_job, SEEK_SET);

    for (i=0; i<db->nr_job; i++)
    {
        memset(&job, 0, sizeof(cjob));
        fread(&job, 1, sizeof(cjob), db->fp);

        db->job[i] = job;
    }

    fprintf(fp_lg, "Job loaded into buffer : %lu\n", (unsigned long int)db->nr_job);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nJob loaded into buffer : %lu\n\n", (unsigned long int)db->nr_job);

    return;
}

/****************************************************************************************
* Liste la table Job depuis le buffer
****************************************************************************************/
void Print_job(dbc *db)
{
    uint64_t i;

    for (i=0; i<db->nr_job; i++)
        Rec_job(&db->job[i]);

    return;
}

/****************************************************************************************
* Imprime un record Job depuis le buffer
****************************************************************************************/
void Rec_job(cjob *rec)
{
    printf("%3d %28s %28s %36s \n",
           rec->id_job,
           rec->nm_lev,
           rec->nm_dep,
           rec->nm_job );

    return;
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
    cjob_recur *tmp_a = (cjob_recur*)a;
    cjob_recur *tmp_b = (cjob_recur*)b;

    return strcmp(tmp_a->job.nm_job, tmp_b->job.nm_job);
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

/************************************************************/
/*  I : /                                                   */
/*  P : Gets the element to the right of the current        */
/*  O : Address of the element to the right                 */
/*          (NULL if current is null)                       */
/************************************************************/
void** job_right(void* current){
    cjob_recur* currentjob = (cjob_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentjob->right;
}

/************************************************************/
/*  I : record to display                                   */
/*      /                                                   */
/*  P : Displays an algo-compatible record                  */
/*  O : /                                                   */
/************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Rec_job_list(void *record, void* nullable){
    cjob_recur* tmp = (cjob_recur*)record;

    Rec_job(&tmp->job);

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the Job               */
/*  O : /                                                   */
/************************************************************/
char* toString_job(void* current){
    cjob_recur *tmp = (cjob_recur*)current;

    return tmp->job.nm_job;
}

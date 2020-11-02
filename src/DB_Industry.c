#include "DB_Industry.h"

/****************************************************************************************/
/*  I : Database to which import the industries CSV file                                */
/*  P : Reads the whole industries CSV file and writes it in the Database file          */
/*  O : /                                                                               */
/****************************************************************************************/
void Import_CSV_industry(dbc *db){
    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	cind ind;
	FILE *fpi, *fp_lg;

	//open DB files
    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    //open Import file
	fpi = fopen(CSV_ind_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nIndustries : importing ...\n");

    //read the first 200 characters in the import file
    fgets(line, 200, fpi);

    //set the DB file pointer to the beginning of the Industries data block
    fseek(db->fp, db->hdr.off_ind, SEEK_SET);

    printf("%lu\n",(unsigned long int)db->hdr.off_ind);

    while (fgets(line, 200, fpi) != NULL)
    {
        //clean the buffer up and set the record type to con
        memset(&ind, 0, sizeof(cind));
        strcpy(ind.tp_rec, "IND");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        //read the industry ID (ptr1) and the sector name (ptr2), then set the industry ID
        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        fld[strlen(ptr1)]='\0';
        ind.id_ind = atoi(fld);                    if (PRT) printf("%d\n", ind.id_ind);

        //set the sector name and read the industry name
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(ind.nm_sec, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", ind.nm_ind);
        ind.nm_sec[strlen(ptr1)]='\0';

        //set the industry name
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(ind.nm_ind, ptr1, strlen(ptr1)-1);        if (PRT) printf("%d\n", ind.id_ind);
        ind.nm_ind[strlen(ptr1)]='\0';

        //write the final record
        fwrite(&ind, 1, sizeof(cind), db->fp);

        i++;
    }

    //save the amount of industries imported
    db->nr_ind = i;

    fprintf(fp_lg, "industries imported : %lu\n", (unsigned long int)db->nr_ind);

    //close all files
    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nindustries imported : %lu\n\n", (unsigned long int)db->nr_ind);

	return ;
}

/****************************************************************************************/
/*  I : Database from which export the industries CSV file                              */
/*  P : Reads the whole industries database and exports it in a CSV file                */
/*  O : /                                                                               */
/****************************************************************************************/
void Export_CSV_industry(dbc *db){
    uint64_t i;
	cind ind;
	FILE *fpo, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    printf("\nIndustry : exporting ...\n");
    fpo = fopen("Data_Export/Exp_industry.csv", "w");
    fprintf(fpo,"Id;Nm_sec;Nm_Ind\n");

    fseek(db->fp, db->hdr.off_ind, SEEK_SET);

    for (i=0; i<db->nr_ind; i++)
    {
        memset(&ind, 0, sizeof(cind));
        fread(&ind, 1, sizeof(cind), db->fp);

        fprintf(fpo,"%d;%s;%s\n",
                ind.id_ind,
                ind.nm_sec,
                ind.nm_ind);
    }

    fprintf(fp_lg, "Industry exported : %lu\n", (unsigned long int)db->nr_ind);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\nIndustry exported : %lu\n\n", (unsigned long int)db->nr_ind);

    return;
}

/****************************************************************************************/
/*  I : Database from which import the records in memory                                */
/*  P : Reads the whole industries database and loads them in memory                    */
/*  O : /                                                                               */
/****************************************************************************************/
void Load_industry(dbc *db){
    uint64_t i;
	cind ind;
	FILE *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    if(db->ind)
        free(db->ind);

    db->ind = (cind*)calloc(db->nr_ind, sizeof(cind));

    printf("\nIndustry : loading ...\n");

    fseek(db->fp, db->hdr.off_ind, SEEK_SET);

    for (i=0; i<db->nr_ind; i++)
    {
        memset(&ind, 0, sizeof(cind));
        fread(&ind, 1, sizeof(cind), db->fp);

        db->ind[i] = ind;
    }

    fprintf(fp_lg, "Industry loaded into buffer : %lu\n", (unsigned long int)db->nr_ind);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nIndustry loaded into buffer : %lu\n\n", (unsigned long int)db->nr_ind);

    return;
}

/****************************************************************************************/
/*  I : industry record to print                                                        */
/*  P : Prints an industry record                                                       */
/*  O : /                                                                               */
/****************************************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Rec_industry(void *rec, void *nullable){
    cind* tmp = (cind*)rec;

    printf("%4d %32s %4s\n",
           tmp->id_ind,
           tmp->nm_sec,
           tmp->nm_ind );

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/****************************************************************************************/
/*  I : First industry to compare                                                       */
/*      Second  industry to compare                                                     */
/*  P : Compares two industries by their PK                                             */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_industry_PK(void* a, void* b){
    cind *tmp_a = (cind*)a;
    cind *tmp_b = (cind*)b;

    if(tmp_a->id_ind > tmp_b->id_ind)
        return 1;
    else if(tmp_a->id_ind < tmp_b->id_ind)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : First industry to compare                                                       */
/*      Second  industry to compare                                                     */
/*  P : Compares two industries by their PK                                             */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_industry_PK_index(void* a, void* b){
    i_cind_PK *tmp_a = (i_cind_PK*)a;
    i_cind_PK *tmp_b = (i_cind_PK*)b;

    if(tmp_a->ind_id > tmp_b->ind_id)
        return 1;
    else if(tmp_a->ind_id < tmp_b->ind_id)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : industry index buffer to which copy data                                        */
/*      industry from which copy data                                                   */
/*  P : Copies all the fields of a industry to a industry index buffer                  */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_industry_index_PK(void* index, void* elem){
    cind* element = (cind*)elem;
    i_cind_PK* i_element = (i_cind_PK*)index;

    if(!index || !element)
        return -1;

    //copy the data from the Industry to the buffer
    i_element->ind_id = element->id_ind;
    strcpy(i_element->tp_rec, "I_INDFK");

    return 0;
}

/****************************************************************************************/
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_industry_index_slot(void* index, uint32_t* offset){
    i_cind_PK* element = (i_cind_PK*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the industry          */
/*  O : /                                                   */
/************************************************************/
char* toString_industry(void* current){
    cind *tmp = (cind*)current;

    return tmp->nm_ind;
}

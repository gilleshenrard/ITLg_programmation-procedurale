#include "DB_Contact.h"

/****************************************************************************************/
/*  I : Database to which import the contacts CSV file                                  */
/*  P : Reads the whole contacts CSV file and writes it in the Database file            */
/*  O : /                                                                               */
/****************************************************************************************/
void Import_CSV_contact(dbc *db){
    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	ccon con;
	FILE *fpi, *fp_lg;

	//open DB files
    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    //open Import file
	fpi = fopen(CSV_con_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nContacts : importing ...\n");

    //read the first 200 characters in the import file
    fgets(line, 200, fpi);

    //set the DB file pointer to the beginning of the Contacts data block
    fseek(db->fp, db->hdr.off_con, SEEK_SET);

    printf("%lu\n",(unsigned long int)db->hdr.off_con);

    while (fgets(line, 200, fpi) != NULL)
    {
        //clean the buffer up and set the record type to con
        memset(&con, 0, sizeof(ccon));
        strcpy(con.tp_rec, "CON");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        //read the campain ID (ptr1) and the company ID (ptr2), then set the campain ID
        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        fld[strlen(ptr1)]='\0';
        con.id_cam = atoi(fld);                    if (PRT) printf("%d\n", con.id_cam);

        //set the company ID and read the job ID
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(fld, ptr1, ptr2-ptr1-1);
        fld[strlen(ptr1)]='\0';
        con.id_cpy = atoi(fld);                    if (PRT) printf("%d\n", con.id_cpy);

        //set the job ID and read the amount of replies
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(fld, ptr1, ptr2-ptr1-1);
        fld[strlen(ptr1)]='\0';
        con.id_job = atoi(fld);                    if (PRT) printf("%d\n", con.id_job);

        //set the amount of replies
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        fld[strlen(ptr1)]='\0';
        con.nr_rep = atof(fld);                      if (PRT) printf("%d\n", con.nr_rep);

        //write the final record
        fwrite(&con, 1, sizeof(ccon), db->fp);

        i++;
    }

    //save the amount of contacts imported
    db->hdr.nr_con = i;

    fprintf(fp_lg, "Contacts imported : %lu\n", (unsigned long int)db->hdr.nr_con);

    //close all files
    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nContacts imported : %lu\n\n", (unsigned long int)db->hdr.nr_con);

	return ;
}

/****************************************************************************************/
/*  I : Database from which export the contacts CSV file                                */
/*  P : Reads the whole contacts database and exports it in a CSV file                  */
/*  O : /                                                                               */
/****************************************************************************************/
void Export_CSV_contact(dbc *db){
    uint64_t i;
	ccon con;
	FILE *fpo, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    printf("\nContact : exporting ...\n");
    fpo = fopen("Data_Export/Exp_contact.csv", "w");
    fprintf(fpo,"Id_Cam;Id_Cpy;Id_Job;Nr_Rep\n");

    fseek(db->fp, db->hdr.off_con, SEEK_SET);

    for (i=0; i<db->hdr.nr_con; i++)
    {
        memset(&con, 0, sizeof(ccon));
        fread(&con, 1, sizeof(ccon), db->fp);

        fprintf(fpo,"%d;%d;%d;%d\n",
                con.id_cam,
                con.id_cpy,
                con.id_job,
                con.nr_rep);
    }

    fprintf(fp_lg, "contact exported : %lu\n", (unsigned long int)db->hdr.nr_con);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\ncontact exported : %lu\n\n", (unsigned long int)db->hdr.nr_con);

    return;
}

/****************************************************************************************/
/*  I : Database from which import the records in memory                                */
/*  P : Reads the whole contacts database and loads them in memory                      */
/*  O : /                                                                               */
/****************************************************************************************/
void Load_contact(dbc *db){
    uint64_t i;
	ccon con;
	FILE *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    if(db->con)
        free(db->con);

    db->con = (ccon*)calloc(db->hdr.nr_con, sizeof(ccon));

    printf("\nContact : loading ...\n");

    fseek(db->fp, db->hdr.off_con, SEEK_SET);

    for (i=0; i<db->hdr.nr_con; i++)
    {
        memset(&con, 0, sizeof(ccon));
        fread(&con, 1, sizeof(ccon), db->fp);

        db->con[i] = con;
    }

    fprintf(fp_lg, "Contacts loaded into buffer : %lu\n", (unsigned long int)db->hdr.nr_con);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nContacts loaded into buffer : %lu\n\n", (unsigned long int)db->hdr.nr_con);

    return;
}

/****************************************************************************************/
/*  I : contact record to print                                                        */
/*  P : Prints an contact record                                                       */
/*  O : /                                                                               */
/****************************************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Rec_contact(void *rec, void *nullable){
    ccon* tmp = (ccon*)rec;

    printf("%4d  %4d  %4d  %4d\n",
            tmp->id_cam,
            tmp->id_cpy,
            tmp->id_job,
            tmp->nr_rep);

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/****************************************************************************************/
/*  I : First contact to compare                                                        */
/*      Second  contact to compare                                                      */
/*  P : Compares two contacts by their PK                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_contact_cpy(void* a, void* b){
    ccon *tmp_a = (ccon*)a;
    ccon *tmp_b = (ccon*)b;

    if(tmp_a->id_cpy > tmp_b->id_cpy)
        return 1;
    else if(tmp_a->id_cpy < tmp_b->id_cpy)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : First contact to compare                                                        */
/*      Second  contact to compare                                                      */
/*  P : Compares two contacts by their PK                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_contact_cpy_index(void* a, void* b){
    i_ccon_cpy *tmp_a = (i_ccon_cpy*)a;
    i_ccon_cpy *tmp_b = (i_ccon_cpy*)b;

    if(tmp_a->cpy_id > tmp_b->cpy_id)
        return 1;
    else if(tmp_a->cpy_id < tmp_b->cpy_id)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : contact index element to compare                                                */
/*      FK of the second contact to compare                                             */
/*  P : Compares two contacts by their FK                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_contact_index_int(void* a, void* b){
    i_ccon_cpy *tmp_a = (i_ccon_cpy*)a;
    int* FK = (int*)b;

    if(tmp_a->cpy_id > *FK)
        return 1;
    else if(tmp_a->cpy_id < *FK)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : contact index buffer to which copy data                                         */
/*      contact from which copy data                                                    */
/*  P : Copies all the fields of a contact to a contact index buffer                    */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_contact_index_cpy(void* index, void* elem){
    ccon* element = (ccon*)elem;
    i_ccon_cpy* i_element = (i_ccon_cpy*)index;

    if(!index || !element)
        return -1;

    //copy the data from the contact to the buffer
    i_element->cpy_id = element->id_cpy;
    strcpy(i_element->tp_rec, "I_CONCA");

    return 0;
}

/****************************************************************************************/
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_contact_index_slot(void* index, uint32_t* offset){
    i_ccon_cpy* element = (i_ccon_cpy*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the contact          */
/*  O : /                                                   */
/************************************************************/
/*char* toString_contact(void* current){
    ccon *tmp = (ccon*)current;

    return tmp->id_cpy;
}*/

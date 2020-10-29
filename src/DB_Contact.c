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

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

	fpi = fopen(CSV_con_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nContacts : importing ...\n");

    fgets(line, 200, fpi);

    fseek(db->fp, db->hdr.off_con, SEEK_SET);

    printf("%lu\n",(unsigned long int)db->hdr.off_con);

    while (fgets(line, 200, fpi) != NULL)
    {
        memset(&con, 0, sizeof(ccon));
        strcpy(con.tp_rec, "CON");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        con.id_cam = atoi(fld);                    if (PRT) printf("%d\n", con.id_cam);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(fld, ptr1, ptr2-ptr1-1);
        con.id_cpy = atoi(fld);                    if (PRT) printf("%d\n", con.id_cpy);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(fld, ptr1, ptr2-ptr1-1);
        con.id_job = atoi(fld);                    if (PRT) printf("%d\n", con.id_job);
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        con.nr_rep = atof(fld);                      if (PRT) printf("%d\n", con.nr_rep);

        fwrite(&con, 1, sizeof(ccon), db->fp);

        i++;
    }

    db->nr_con = i;

    fprintf(fp_lg, "Contacts imported : %lu\n", (unsigned long int)db->nr_con);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nContacts imported : %lu\n\n", (unsigned long int)db->nr_con);

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

    for (i=0; i<db->nr_con; i++)
    {
        memset(&con, 0, sizeof(ccon));
        fread(&con, 1, sizeof(ccon), db->fp);

        fprintf(fpo,"%d;%d;%d;%d\n",
                con.id_cam,
                con.id_cpy,
                con.id_job,
                con.nr_rep);
    }

    fprintf(fp_lg, "contact exported : %lu\n", (unsigned long int)db->nr_con);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\ncontact exported : %lu\n\n", (unsigned long int)db->nr_con);

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

    db->con = (ccon*)calloc(db->nr_con, sizeof(ccon));

    printf("\nContact : loading ...\n");

    fseek(db->fp, db->hdr.off_con, SEEK_SET);

    for (i=0; i<db->nr_con; i++)
    {
        memset(&con, 0, sizeof(ccon));
        fread(&con, 1, sizeof(ccon), db->fp);

        db->con[i] = con;
    }

    fprintf(fp_lg, "Contacts loaded into buffer : %lu\n", (unsigned long int)db->nr_con);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nContacts loaded into buffer : %lu\n\n", (unsigned long int)db->nr_con);

    return;
}

/****************************************************************************************/
/*  I : Database from which print the contacts                                          */
/*  P : Prints all the contacts in the memory buffer                                    */
/*  O : /                                                                               */
/****************************************************************************************/
void Print_contact(dbc *db){
    uint64_t i;

    for (i=0; i<db->nr_con; i++)
        Rec_contact(&db->con[i]);

    return;
}

/****************************************************************************************/
/*  I : contact record to print                                                        */
/*  P : Prints an contact record                                                       */
/*  O : /                                                                               */
/****************************************************************************************/
void Rec_contact(ccon *rec){
    printf("%4d  %4d  %4d  %4d\n",
            rec->id_cam,
            rec->id_cpy,
            rec->id_job,
            rec->nr_rep);

    return;
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
    ccon_recur *tmp_a = (ccon_recur*)a;
    ccon_recur *tmp_b = (ccon_recur*)b;

    if(tmp_a->con.id_cpy > tmp_b->con.id_cpy)
        return 1;
    else if(tmp_a->con.id_cpy < tmp_b->con.id_cpy)
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
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_contact_index_slot(void* index, void* offset){
    i_ccon_cpy* element = (i_ccon_cpy*)index;
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
void** contact_right(void* current){
    ccon_recur* currentcon = (ccon_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentcon->right;
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
int Rec_contact_list(void *record, void* nullable){
    ccon_recur* tmp = (ccon_recur*)record;

    Rec_contact(&tmp->con);

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the contact          */
/*  O : /                                                   */
/************************************************************/
/*char* toString_contact(void* current){
    ccon_recur *tmp = (ccon_recur*)current;

    return tmp->con.id_cpy;
}*/

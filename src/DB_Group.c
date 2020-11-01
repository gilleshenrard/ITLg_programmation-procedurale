#include "DB_Group.h"

/****************************************************************************************/
/*  I : Database to which import the groups CSV file                                    */
/*  P : Reads the whole groups CSV file and writes it in the Database file              */
/*  O : /                                                                               */
/****************************************************************************************/
void Import_CSV_Group(dbc *db){
    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	cgrp grp;
	FILE *fpi, *fp_lg;

	//open DB files
    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    //open Import file
	fpi = fopen(CSV_grp_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nGroups : importing ...\n");

    //read the first 200 characters in the import file
    fgets(line, 200, fpi);

    //set the DB file pointer to the beginning of the Groups data block
    fseek(db->fp, db->hdr.off_grp, SEEK_SET);

    printf("%lu\n",(unsigned long int)db->hdr.off_grp);

    while (fgets(line, 200, fpi) != NULL)
    {
        //clean the buffer up and set the record type to grp
        memset(&grp, 0, sizeof(cgrp));
        strcpy(grp.tp_rec, "GRP");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        //read the group ID (ptr1) and the group name (ptr2), then set the group ID
        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        fld[strlen(ptr1)]='\0';
        grp.id_grp = atoi(fld);                    if (PRT) printf("%d\n", grp.id_grp);

        //set the group name and read the group country
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(grp.nm_grp, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", grp.nm_grp);
        grp.nm_grp[strlen(ptr1)]='\0';

        //set the group country and read the group country ID
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(grp.cd_cty, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", grp.cd_cty);
        grp.cd_cty[strlen(ptr1)]='\0';

        //set the group country ID
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        fld[strlen(ptr1)]='\0';
        grp.id_cty = atoi(fld);                    if (PRT) printf("%d\n", grp.id_cty);

        //write the final record
        fwrite(&grp, 1, sizeof(cgrp), db->fp);

        i++;
    }

    //save the amount of groups imported
    db->nr_grp = i;

    fprintf(fp_lg, "Groups imported : %lu\n", (unsigned long int)db->nr_grp);

    //close all files
    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nGroups imported : %lu\n\n", (unsigned long int)db->nr_grp);

	return ;
}

/****************************************************************************************/
/*  I : Database from which export the groups CSV file                                  */
/*  P : Reads the whole groups database and exports it in a CSV file                    */
/*  O : /                                                                               */
/****************************************************************************************/
void Export_CSV_Group(dbc *db){
    uint64_t i;
	cgrp grp;
	FILE *fpo, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    printf("\nCountry : exporting ...\n");
    fpo = fopen("Data_Export/Exp_Group.csv", "w");
    fprintf(fpo,"Id;Nm_grp;Nm_Zon;Cd_Iso\n");

    fseek(db->fp, db->hdr.off_grp, SEEK_SET);

    for (i=0; i<db->nr_grp; i++)
    {
        memset(&grp, 0, sizeof(cgrp));
        fread(&grp, 1, sizeof(cgrp), db->fp);

        fprintf(fpo,"%d;%s;%s;%d\n",
                grp.id_grp,
                grp.nm_grp,
                grp.cd_cty,
                grp.id_cty);
    }

    fprintf(fp_lg, "Country exported : %lu\n", (unsigned long int)db->nr_grp);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\nCountry exported : %lu\n\n", (unsigned long int)db->nr_grp);

    return;
}

/****************************************************************************************/
/*  I : Database from which import the records in memory                                */
/*  P : Reads the whole groups database and loads them in memory                        */
/*  O : /                                                                               */
/****************************************************************************************/
void Load_Group(dbc *db){
    uint64_t i;
	cgrp grp;
	FILE *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    if(db->grp)
        free(db->grp);

    db->grp = (cgrp*)calloc(db->nr_grp, sizeof(cgrp));

    printf("\nGroup : loading ...\n");

    fseek(db->fp, db->hdr.off_grp, SEEK_SET);

    for (i=0; i<db->nr_grp; i++)
    {
        memset(&grp, 0, sizeof(cgrp));
        fread(&grp, 1, sizeof(cgrp), db->fp);

        db->grp[i] = grp;
    }

    fprintf(fp_lg, "Country loaded into buffer : %lu\n", (unsigned long int)db->nr_grp);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nCountry loaded into buffer : %lu\n\n", (unsigned long int)db->nr_grp);

    return;
}

/****************************************************************************************/
/*  I : Database from which print the groups                                            */
/*  P : Prints all the groups in the memory buffer                                      */
/*  O : /                                                                               */
/****************************************************************************************/
void Print_Group(dbc *db){
    uint64_t i;

    for (i=0; i<db->nr_grp; i++)
        Rec_Group(&db->grp[i]);

    return;
}

/****************************************************************************************/
/*  I : Group record to print                                                           */
/*  P : Prints a group record                                                           */
/*  O : /                                                                               */
/****************************************************************************************/
void Rec_Group(cgrp *rec){
    printf("%4d %32s %4s %3d \n",
           rec->id_grp,
           rec->nm_grp,
           rec->cd_cty,
           rec->id_cty );

    return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/****************************************************************************************/
/*  I : First group to compare                                                          */
/*      Second  group to compare                                                        */
/*  P : Compares two groups by their PK                                                 */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_group_FK(void* a, void* b){
    cgrp_recur *tmp_a = (cgrp_recur*)a;
    cgrp_recur *tmp_b = (cgrp_recur*)b;

    if(tmp_a->grp.id_cty > tmp_b->grp.id_cty)
        return 1;
    else if(tmp_a->grp.id_cty < tmp_b->grp.id_cty)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : First group to compare                                                          */
/*      Second  group to compare                                                        */
/*  P : Compares two groups by their name                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_group_nm(void* a, void* b){
    cgrp_recur *tmp_a = (cgrp_recur*)a;
    cgrp_recur *tmp_b = (cgrp_recur*)b;

    return strcmp(tmp_a->grp.nm_grp, tmp_b->grp.nm_grp);
}

/****************************************************************************************/
/*  I : First group to compare                                                          */
/*      Second  group to compare                                                        */
/*  P : Compares two groups by their PK                                                 */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_group_FK_index(void* a, void* b){
    i_cgrp_FK *tmp_a = (i_cgrp_FK*)a;
    i_cgrp_FK *tmp_b = (i_cgrp_FK*)b;

    if(tmp_a->cty_id > tmp_b->cty_id)
        return 1;
    else if(tmp_a->cty_id < tmp_b->cty_id)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : First group to compare                                                          */
/*      Second  group to compare                                                        */
/*  P : Compares two groups by their name                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_group_nm_index(void* a, void* b){
    i_cgrp_nm *tmp_a = (i_cgrp_nm*)a;
    i_cgrp_nm *tmp_b = (i_cgrp_nm*)b;

    return strcmp(tmp_a->nm_grp, tmp_b->nm_grp);
}

/****************************************************************************************/
/*  I : Group index buffer to which copy data                                           */
/*      Group from which copy data                                                      */
/*  P : Copies all the fields of a group to a group index buffer                        */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_group_index_FK(void* index, void* elem){
    cgrp* element = (cgrp*)elem;
    i_cgrp_FK* i_element = (i_cgrp_FK*)index;

    if(!index || !element)
        return -1;

    //copy the data from the country to the buffer
    i_element->cty_id = element->id_cty;
    strcpy(i_element->tp_rec, "I_GRPFK");

    return 0;
}

/****************************************************************************************/
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_group_index_slot(void* index, uint32_t* offset){
    i_cgrp_FK* element = (i_cgrp_FK*)index;
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
void** group_right(void* current){
    cgrp_recur* currentGrp = (cgrp_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentGrp->right;
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
int Rec_group_list(void *record, void* nullable){
    cgrp_recur* tmp = (cgrp_recur*)record;

    Rec_Group(&tmp->grp);

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the group             */
/*  O : /                                                   */
/************************************************************/
char* toString_group(void* current){
    cgrp_recur *tmp = (cgrp_recur*)current;

    return tmp->grp.nm_grp;
}

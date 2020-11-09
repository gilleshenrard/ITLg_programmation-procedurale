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
    db->hdr.nr_grp = i;

    fprintf(fp_lg, "Groups imported : %lu\n", (unsigned long int)db->hdr.nr_grp);

    //close all files
    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nGroups imported : %lu\n\n", (unsigned long int)db->hdr.nr_grp);

	return ;
}

/****************************************************************************************/
/*  I : Group to format to CSV                                                          */
/*      Buffer in which the final CSV line will be stored                               */
/*  P : Formats a Group to a CSV file line                                              */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVFormatGroup(void* elem, char* finalLine){
    cgrp* grp = (cgrp*)elem;

    sprintf(finalLine,"%d;%s;%s;%d\n",
            grp->id_grp,
            grp->nm_grp,
            grp->cd_cty,
            grp->id_cty);

    return 0;
}

/****************************************************************************************/
/*  I : Group record to print                                                           */
/*  P : Prints a group record                                                           */
/*  O : /                                                                               */
/****************************************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Rec_Group(void *rec, void *nullable){
    cgrp* tmp = (cgrp*)rec;

    printf("%4d %32s %4s %3d \n",
           tmp->id_grp,
           tmp->nm_grp,
           tmp->cd_cty,
           tmp->id_cty );

    return 0;
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
    cgrp *tmp_a = (cgrp*)a;
    cgrp *tmp_b = (cgrp*)b;

    if(tmp_a->id_cty > tmp_b->id_cty)
        return 1;
    else if(tmp_a->id_cty < tmp_b->id_cty)
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
    cgrp *tmp_a = (cgrp*)a;
    cgrp *tmp_b = (cgrp*)b;

    return strcmp(tmp_a->nm_grp, tmp_b->nm_grp);
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
int compare_group_nm_index_char(void* a, void* b){
    i_cgrp_nm *tmp_a = (i_cgrp_nm*)a;
    char *tmp_b = (char*)b;

    return strcmp(tmp_a->nm_grp, tmp_b);
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
/*  I : Group index buffer to which copy data                                           */
/*      Group from which copy data                                                      */
/*  P : Copies all the fields of a group to a group index buffer                        */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_group_index_nm(void* index, void* elem){
    cgrp* element = (cgrp*)elem;
    i_cgrp_nm* i_element = (i_cgrp_nm*)index;

    if(!index || !element)
        return -1;

    //copy the data from the country to the buffer
    strcpy(i_element->nm_grp, element->nm_grp);
    strcpy(i_element->tp_rec, "I_GRPNM");

    return 0;
}

/****************************************************************************************/
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_group_FK_index_slot(void* index, uint32_t* offset){
    i_cgrp_FK* element = (i_cgrp_FK*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

/****************************************************************************************/
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_group_nm_index_slot(void* index, uint32_t* offset){
    i_cgrp_nm* element = (i_cgrp_nm*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the group             */
/*  O : /                                                   */
/************************************************************/
char* toString_group(void* current){
    cgrp *tmp = (cgrp*)current;

    return tmp->nm_grp;
}

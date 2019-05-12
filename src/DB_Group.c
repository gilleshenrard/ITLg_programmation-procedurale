#include "../lib/DB_Group.h"

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

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

	fpi = fopen(CSV_grp_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nGroups : importing ...\n");

    fgets(line, 200, fpi);

    fseek(db->fp, db->hdr.off_grp, SEEK_SET);

    printf("%08lX\n",db->hdr.off_grp);

    while (fgets(line, 200, fpi) != NULL)
    {
        memset(&grp, 0, sizeof(cgrp));
        strcpy(grp.tp_rec, "GRP");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        grp.id_grp = atoi(fld);                    if (PRT) printf("%d\n", grp.id_grp);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(grp.nm_grp, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", grp.nm_grp);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(grp.cd_cty, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", grp.cd_cty);
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        grp.id_cty = atoi(fld);                    if (PRT) printf("%d\n", grp.id_cty);

        fwrite(&grp, 1, sizeof(cgrp), db->fp);

        i++;
    }

    db->nr_grp = i;

    fprintf(fp_lg, "Groups imported : %d \n", db->nr_grp);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nGroups imported : %d \n\n", db->nr_grp);

	return ;
}

/****************************************************************************************/
/*  I : Database from which export the groups CSV file                                  */
/*  P : Reads the whole groups database and exports it in a CSV file                    */
/*  O : /                                                                               */
/****************************************************************************************/
void Export_CSV_Group(dbc *db){
    int i;
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

    fprintf(fp_lg, "Country exported : %d \n", db->nr_grp);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\nCountry exported : %d \n\n", db->nr_grp);

    return;
}

/****************************************************************************************/
/*  I : Database from which import the records in memory                                */
/*  P : Reads the whole groups database and loads them in memory                        */
/*  O : /                                                                               */
/****************************************************************************************/
void Load_Group(dbc *db){
    int i;
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

    fprintf(fp_lg, "Country loaded into buffer : %d \n", db->nr_grp);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nCountry loaded into buffer : %d \n\n", db->nr_grp);

    return;
}

/****************************************************************************************/
/*  I : Database from which print the groups                                            */
/*  P : Prints all the groups in the memory buffer                                      */
/*  O : /                                                                               */
/****************************************************************************************/
void Print_Group(dbc *db){
    int i;

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
/*  I : /                                                                               */
/*  P : Allocates memory for a group and sets its height to 1 (leaf for AVL)            */
/*  O : Group created if OK                                                             */
/*      NULL if error                                                                   */
/****************************************************************************************/
void* allocate_group(void){
    cgrp_recur *tmp=NULL;

    //memory allocation for the new element (calloc to initialize with all 0)
    tmp = calloc(1, sizeof(cgrp_recur));
    if(tmp) tmp->height = 1;

    return tmp;
}

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
/*      FK of the second group to compare                                               */
/*  P : Compares two groups by their FK                                                 */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_group_FK_int(void* a, void* b){
    cgrp_recur *tmp_a = (cgrp_recur*)a;
    int* fk = (int*)b;

    if(tmp_a->grp.id_cty > *fk)
        return 1;
    else if(tmp_a->grp.id_cty < *fk)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : Group index element to compare                                                  */
/*      FK of the second group to compare                                               */
/*  P : Compares two groups by their FK                                                 */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_group_index_int(void* a, void* b){
    i_cgrp_FK *tmp_a = (i_cgrp_FK*)a;
    int* fk = (int*)b;

    if(tmp_a->cty_id > *fk)
        return 1;
    else if(tmp_a->cty_id < *fk)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : Group to which copy data                                                        */
/*      Group from which copy data                                                      */
/*  P : Copies all the fields of groups from new to old                                 */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_group(void* oldelem, void* newelem){
    cgrp_recur* oldTuple = (cgrp_recur*)oldelem;
    cgrp_recur* newTuple = (cgrp_recur*)newelem;
    cgrp_recur *saveRight = NULL, *saveLeft=NULL;

    if(!oldelem || !newelem)
        return -1;

    //save the pointer values of the old country
    saveRight = oldTuple->right;
    saveLeft = oldTuple->left;

    //copy the data from the new country to the old one
    *oldTuple = *newTuple;

    //restore the pointer values
    oldTuple->right = saveRight;
    oldTuple->left = saveLeft;

    return 0;
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
int assign_group_index_slot(void* index, void* offset){
    i_cgrp_FK* element = (i_cgrp_FK*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

/************************************************************/
/*  I : Groups to swap                                      */
/*  P : Swaps two groups                                    */
/*  O : 0 -> Swapped                                        */
/*     -1 -> Error                                          */
/************************************************************/
int swap_group(void* first, void* second){
    cgrp_recur tmp;

    if(!first || !second)
        return -1;

    memset(&tmp, 0, sizeof(cgrp_recur));

    assign_group((void*)&tmp, first);
    assign_group(first, second);
    assign_group(second, (void*)&tmp);

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
/*  I : /                                                   */
/*  P : Gets the element to the left of the current         */
/*  O : Address of the element to the left                  */
/*          (NULL if current is null)                       */
/************************************************************/
void** group_left(void* current){
    cgrp_recur* currentGrp = (cgrp_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentGrp->left;
}

/************************************************************/
/*  I : record to display                                   */
/*      /                                                   */
/*  P : Displays an algo-compatible record                  */
/*  O : /                                                   */
/************************************************************/
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

/************************************************************/
/*  I : Group AVL leaf of which to get the height           */
/*  P : Gets the height of the current AVL leaf             */
/*  O : Leaf height                                         */
/************************************************************/
int get_group_height(void* current){
    cgrp_recur *tmp = (cgrp_recur*)current;


    return (tmp == NULL ? 0 : tmp->height);
}

/************************************************************/
/*  I : Group AVL leaf of which to set the height           */
/*      New value for the height                            */
/*  P : Sets the height of the current AVL leaf             */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
int set_group_height(void* current, int value){
    cgrp_recur *tmp = (cgrp_recur*)current;

    tmp->height = value;

    return 0;
}

/************************************************************/
/*  I : Group AVL leaf to free                              */
/*      /                                                   */
/*  P : Frees the memory for the current group              */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
void* free_group(void* group, void* nullable){
    free(group);
    return 0;
}

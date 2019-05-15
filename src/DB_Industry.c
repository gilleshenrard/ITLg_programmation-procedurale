#include "../lib/DB_Industry.h"

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

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

	fpi = fopen(CSV_ind_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nIndustries : importing ...\n");

    fgets(line, 200, fpi);

    fseek(db->fp, db->hdr.off_ind, SEEK_SET);

    printf("%08lX\n",db->hdr.off_ind);

    while (fgets(line, 200, fpi) != NULL)
    {
        memset(&ind, 0, sizeof(cind));
        strcpy(ind.tp_rec, "IND");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        ind.id_ind = atoi(fld);                    if (PRT) printf("%d\n", ind.id_ind);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(ind.nm_sec, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", ind.nm_ind);
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(ind.nm_ind, ptr1, strlen(ptr1)-1);        if (PRT) printf("%d\n", ind.id_ind);

        fwrite(&ind, 1, sizeof(cind), db->fp);

        i++;
    }

    db->nr_ind = i;

    fprintf(fp_lg, "industries imported : %d \n", db->nr_ind);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nindustries imported : %d \n\n", db->nr_ind);

	return ;
}

/****************************************************************************************/
/*  I : Database from which export the industries CSV file                              */
/*  P : Reads the whole industries database and exports it in a CSV file                */
/*  O : /                                                                               */
/****************************************************************************************/
void Export_CSV_industry(dbc *db){
    int i;
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

    fprintf(fp_lg, "Industry exported : %d \n", db->nr_ind);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\nIndustry exported : %d \n\n", db->nr_ind);

    return;
}

/****************************************************************************************/
/*  I : Database from which import the records in memory                                */
/*  P : Reads the whole industries database and loads them in memory                    */
/*  O : /                                                                               */
/****************************************************************************************/
void Load_industry(dbc *db){
    int i;
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

    fprintf(fp_lg, "Industry loaded into buffer : %d \n", db->nr_ind);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nIndustry loaded into buffer : %d \n\n", db->nr_ind);

    return;
}

/****************************************************************************************/
/*  I : Database from which print the industries                                        */
/*  P : Prints all the industries in the memory buffer                                  */
/*  O : /                                                                               */
/****************************************************************************************/
void Print_industry(dbc *db){
    int i;

    for (i=0; i<db->nr_ind; i++)
        Rec_industry(&db->ind[i]);

    return;
}

/****************************************************************************************/
/*  I : industry record to print                                                        */
/*  P : Prints an industry record                                                       */
/*  O : /                                                                               */
/****************************************************************************************/
void Rec_industry(cind *rec){
    printf("%4d %32s %4s\n",
           rec->id_ind,
           rec->nm_sec,
           rec->nm_ind );

    return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/****************************************************************************************/
/*  I : /                                                                               */
/*  P : Allocates memory for a industry and sets its height to 1 (leaf for AVL)         */
/*  O : industry created if OK                                                          */
/*      NULL if error                                                                   */
/****************************************************************************************/
void* allocate_industry(void){
    cind_recur *tmp=NULL;

    //memory allocation for the new element (calloc to initialize with all 0)
    tmp = calloc(1, sizeof(cind_recur));
    if(tmp) tmp->height = 1;

    return tmp;
}

/****************************************************************************************/
/*  I : First industry to compare                                                       */
/*      Second  industry to compare                                                     */
/*  P : Compares two industries by their PK                                             */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_industry_PK(void* a, void* b){
    cind_recur *tmp_a = (cind_recur*)a;
    cind_recur *tmp_b = (cind_recur*)b;

    if(tmp_a->ind.id_ind > tmp_b->ind.id_ind)
        return 1;
    else if(tmp_a->ind.id_ind < tmp_b->ind.id_ind)
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
/*  I : First industry to compare                                                       */
/*      FK of the second industry to compare                                            */
/*  P : Compares two industries by their FK                                             */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_industry_PK_int(void* a, void* b){
    cind_recur *tmp_a = (cind_recur*)a;
    int* fk = (int*)b;

    if(tmp_a->ind.id_ind > *fk)
        return 1;
    else if(tmp_a->ind.id_ind < *fk)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : industry index element to compare                                               */
/*      FK of the second industry to compare                                            */
/*  P : Compares two industries by their FK                                             */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_industry_index_int(void* a, void* b){
    i_cind_PK *tmp_a = (i_cind_PK*)a;
    int* fk = (int*)b;

    if(tmp_a->ind_id > *fk)
        return 1;
    else if(tmp_a->ind_id < *fk)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : industry to which copy data                                                     */
/*      industry from which copy data                                                   */
/*  P : Copies all the fields of industries from new to old                             */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_industry(void* oldelem, void* newelem){
    cind_recur* oldTuple = (cind_recur*)oldelem;
    cind_recur* newTuple = (cind_recur*)newelem;
    cind_recur *saveRight = NULL, *saveLeft=NULL;

    if(!oldelem || !newelem)
        return -1;

    //save the pointer values of the old Industry
    saveRight = oldTuple->right;
    saveLeft = oldTuple->left;

    //copy the data from the new Industry to the old one
    *oldTuple = *newTuple;

    //restore the pointer values
    oldTuple->right = saveRight;
    oldTuple->left = saveLeft;

    return 0;
}

/****************************************************************************************/
/*  I : industry to which copy data                                                     */
/*      industry from which copy data                                                   */
/*  P : Copies all the fields of industries from new to old                             */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_industry_index(void* oldelem, void* newelem){
    i_cind_PK* oldTuple = (i_cind_PK*)oldelem;
    i_cind_PK* newTuple = (i_cind_PK*)newelem;

    if(!oldelem || !newelem)
        return -1;

    //copy the data from the new Industry to the old one
    *oldTuple = *newTuple;

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
int assign_industry_index_slot(void* index, void* offset){
    i_cind_PK* element = (i_cind_PK*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

/************************************************************/
/*  I : industries to swap                                  */
/*  P : Swaps two industries                                */
/*  O : 0 -> Swapped                                        */
/*     -1 -> Error                                          */
/************************************************************/
int swap_industry(void* first, void* second){
    cind_recur tmp;

    if(!first || !second)
        return -1;

    memset(&tmp, 0, sizeof(cind_recur));

    assign_industry((void*)&tmp, first);
    assign_industry(first, second);
    assign_industry(second, (void*)&tmp);

    return 0;
}

/************************************************************/
/*  I : industries to swap                                  */
/*  P : Swaps two industries                                */
/*  O : 0 -> Swapped                                        */
/*     -1 -> Error                                          */
/************************************************************/
int swap_industry_index(void* first, void* second){
    i_cind_PK tmp;

    if(!first || !second)
        return -1;

    memset(&tmp, 0, sizeof(i_cind_PK));

    assign_industry_index((void*)&tmp, first);
    assign_industry_index(first, second);
    assign_industry_index(second, (void*)&tmp);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Gets the element to the right of the current        */
/*  O : Address of the element to the right                 */
/*          (NULL if current is null)                       */
/************************************************************/
void** industry_right(void* current){
    cind_recur* currentind = (cind_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentind->right;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Gets the element to the left of the current         */
/*  O : Address of the element to the left                  */
/*          (NULL if current is null)                       */
/************************************************************/
void** industry_left(void* current){
    cind_recur* currentind = (cind_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentind->left;
}

/************************************************************/
/*  I : record to display                                   */
/*      /                                                   */
/*  P : Displays an algo-compatible record                  */
/*  O : /                                                   */
/************************************************************/
int Rec_industry_list(void *record, void* nullable){
    cind_recur* tmp = (cind_recur*)record;

    Rec_industry(&tmp->ind);

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the industry          */
/*  O : /                                                   */
/************************************************************/
char* toString_industry(void* current){
    cind_recur *tmp = (cind_recur*)current;

    return tmp->ind.nm_ind;
}

/************************************************************/
/*  I : industry AVL leaf of which to get the height        */
/*  P : Gets the height of the current AVL leaf             */
/*  O : Leaf height                                         */
/************************************************************/
int get_industry_height(void* current){
    cind_recur *tmp = (cind_recur*)current;

    return (tmp == NULL ? 0 : tmp->height);
}

/************************************************************/
/*  I : industry AVL leaf of which to set the height        */
/*      New value for the height                            */
/*  P : Sets the height of the current AVL leaf             */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
int set_industry_height(void* current, int value){
    cind_recur *tmp = (cind_recur*)current;

    tmp->height = value;

    return 0;
}

/************************************************************/
/*  I : industry AVL leaf to free                           */
/*      /                                                   */
/*  P : Frees the memory for the current industry           */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
void* free_industry(void* industry, void* nullable){
    free(industry);
    return 0;
}

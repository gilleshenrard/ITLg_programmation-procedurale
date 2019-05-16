#include "../lib/DB_Campain.h"

/****************************************************************************************/
/*  I : Database to which import the campains CSV file                                  */
/*  P : Reads the whole campains CSV file and writes it in the Database file            */
/*  O : /                                                                               */
/****************************************************************************************/
void Import_CSV_campain(dbc *db){
    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	ccam cam;
	FILE *fpi, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

	fpi = fopen(CSV_cam_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\ncampains : importing ...\n");

    fgets(line, 200, fpi);

    fseek(db->fp, db->hdr.off_cam, SEEK_SET);

    printf("%08lX\n",db->hdr.off_cam);

    while (fgets(line, 200, fpi) != NULL)
    {
        memset(&cam, 0, sizeof(ccam));
        strcpy(cam.tp_rec, "cam");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cam.id_cam = atoi(fld);                    if (PRT) printf("%d\n", cam.id_cam);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.nm_cam, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.tp_cam, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.dt_cam, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.nm_lev, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.nm_dep, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.nm_sec, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.nm_zon, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cam.nr_year = atoi(fld);                   if (PRT) printf("%d\n", cam.id_cam);
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        cam.cost = atof(fld);                      if (PRT) printf("%d\n", cam.id_cam);

        fwrite(&cam, 1, sizeof(ccam), db->fp);

        i++;
    }

    db->nr_cam = i;

    fprintf(fp_lg, "campains imported : %d \n", db->nr_cam);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\ncampains imported : %d \n\n", db->nr_cam);

	return ;
}

/****************************************************************************************/
/*  I : Database from which export the campains CSV file                                */
/*  P : Reads the whole campains database and exports it in a CSV file                  */
/*  O : /                                                                               */
/****************************************************************************************/
void Export_CSV_campain(dbc *db){
    int i;
	ccam cam;
	FILE *fpo, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    printf("\nCampain : exporting ...\n");
    fpo = fopen("Data_Export/Exp_Campain.csv", "w");
    fprintf(fpo,"Id;Nm_Cam;Tp_Cam;Dt_Cam;Nm_Lev;Nm_Dep;Nm_Sec;Nm_Zon;Nr_Yr;Cost\n");

    fseek(db->fp, db->hdr.off_cam, SEEK_SET);

    for (i=0; i<db->nr_cam; i++)
    {
        memset(&cam, 0, sizeof(ccam));
        fread(&cam, 1, sizeof(ccam), db->fp);

        fprintf(fpo,"%d;%s;%s;%s;%s;%s;%s;%s;%d;%f\n",
                cam.id_cam,
                cam.nm_cam,
                cam.tp_cam,
                cam.dt_cam,
                cam.nm_lev,
                cam.nm_dep,
                cam.nm_sec,
                cam.nm_zon,
                cam.nr_year,
                cam.cost);
    }

    fprintf(fp_lg, "Campain exported : %d \n", db->nr_cam);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\nCampain exported : %d \n\n", db->nr_cam);

    return;
}

/****************************************************************************************/
/*  I : Database from which import the records in memory                                */
/*  P : Reads the whole campains database and loads them in memory                      */
/*  O : /                                                                               */
/****************************************************************************************/
void Load_campain(dbc *db){
    int i;
	ccam cam;
	FILE *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    if(db->cam)
        free(db->cam);

    db->cam = (ccam*)calloc(db->nr_cam, sizeof(ccam));

    printf("\nCampain : loading ...\n");

    fseek(db->fp, db->hdr.off_cam, SEEK_SET);

    for (i=0; i<db->nr_cam; i++)
    {
        memset(&cam, 0, sizeof(ccam));
        fread(&cam, 1, sizeof(ccam), db->fp);

        db->cam[i] = cam;
    }

    fprintf(fp_lg, "Campain loaded into buffer : %d \n", db->nr_cam);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nCampain loaded into buffer : %d \n\n", db->nr_cam);

    return;
}

/****************************************************************************************/
/*  I : Database from which print the campains                                          */
/*  P : Prints all the campains in the memory buffer                                    */
/*  O : /                                                                               */
/****************************************************************************************/
void Print_campain(dbc *db){
    int i;

    for (i=0; i<db->nr_cam; i++)
        Rec_campain(&db->cam[i]);

    return;
}

/****************************************************************************************/
/*  I : Campain record to print                                                        */
/*  P : Prints an Campain record                                                       */
/*  O : /                                                                               */
/****************************************************************************************/
void Rec_campain(ccam *rec){
    printf("%4d %32s %32s %32s %32s %32s %32s %32s %4d %f\n",
            rec->id_cam,
            rec->nm_cam,
            rec->tp_cam,
            rec->dt_cam,
            rec->nm_lev,
            rec->nm_dep,
            rec->nm_sec,
            rec->nm_zon,
            rec->nr_year,
            rec->cost);

    return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/****************************************************************************************/
/*  I : /                                                                               */
/*  P : Allocates memory for a Campain and sets its height to 1 (leaf for AVL)          */
/*  O : Campain created if OK                                                           */
/*      NULL if error                                                                   */
/****************************************************************************************/
void* allocate_campain(void){
    ccam_recur *tmp=NULL;

    //memory allocation for the new element (calloc to initialize with all 0)
    tmp = calloc(1, sizeof(ccam_recur));
    if(tmp) tmp->height = 1;

    return tmp;
}

/****************************************************************************************/
/*  I : First campain to compare                                                        */
/*      Second  campain to compare                                                      */
/*  P : Compares two campains by their PK                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_campain_PK(void* a, void* b){
    ccam_recur *tmp_a = (ccam_recur*)a;
    ccam_recur *tmp_b = (ccam_recur*)b;

    if(tmp_a->cam.id_cam > tmp_b->cam.id_cam)
        return 1;
    else if(tmp_a->cam.id_cam < tmp_b->cam.id_cam)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : First campain to compare                                                        */
/*      Second  campain to compare                                                      */
/*  P : Compares two campains by their PK                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_campain_PK_index(void* a, void* b){
    i_ccam_PK *tmp_a = (i_ccam_PK*)a;
    i_ccam_PK *tmp_b = (i_ccam_PK*)b;

    if(tmp_a->cam_id > tmp_b->cam_id)
        return 1;
    else if(tmp_a->cam_id < tmp_b->cam_id)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : First campain to compare                                                        */
/*      PK of the second campain to compare                                             */
/*  P : Compares two campains by their PK                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_campain_PK_int(void* a, void* b){
    ccam_recur *tmp_a = (ccam_recur*)a;
    int* PK = (int*)b;

    if(tmp_a->cam.id_cam > *PK)
        return 1;
    else if(tmp_a->cam.id_cam < *PK)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : campain index element to compare                                                */
/*      PK of the second campain to compare                                             */
/*  P : Compares two campains by their PK                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_campain_index_int(void* a, void* b){
    i_ccam_PK *tmp_a = (i_ccam_PK*)a;
    int* PK = (int*)b;

    if(tmp_a->cam_id > *PK)
        return 1;
    else if(tmp_a->cam_id < *PK)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : campain to which copy data                                                      */
/*      campain from which copy data                                                    */
/*  P : Copies all the fields of campains from new to old                               */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_campain(void* oldelem, void* newelem){
    ccam_recur* oldTuple = (ccam_recur*)oldelem;
    ccam_recur* newTuple = (ccam_recur*)newelem;
    ccam_recur *saveRight = NULL, *saveLeft=NULL;

    if(!oldelem || !newelem)
        return -1;

    //save the pointer values of the old campain
    saveRight = oldTuple->right;
    saveLeft = oldTuple->left;

    //copy the data from the new campain to the old one
    *oldTuple = *newTuple;

    //restore the pointer values
    oldTuple->right = saveRight;
    oldTuple->left = saveLeft;

    return 0;
}

/****************************************************************************************/
/*  I : campain to which copy data                                                      */
/*      campain from which copy data                                                    */
/*  P : Copies all the fields of campains from new to old                               */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_campain_index(void* oldelem, void* newelem){
    i_ccam_PK* oldTuple = (i_ccam_PK*)oldelem;
    i_ccam_PK* newTuple = (i_ccam_PK*)newelem;

    if(!oldelem || !newelem)
        return -1;

    //copy the data from the new campain to the old one
    *oldTuple = *newTuple;

    return 0;
}

/****************************************************************************************/
/*  I : campain index buffer to which copy data                                         */
/*      campain from which copy data                                                    */
/*  P : Copies all the fields of a campain to a campain index buffer                    */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_campain_index_PK(void* index, void* elem){
    ccam* element = (ccam*)elem;
    i_ccam_PK* i_element = (i_ccam_PK*)index;

    if(!index || !element)
        return -1;

    //copy the data from the campain to the buffer
    i_element->cam_id = element->id_cam;
    strcpy(i_element->tp_rec, "I_CAMPK");

    return 0;
}

/****************************************************************************************/
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_campain_index_slot(void* index, void* offset){
    i_ccam_PK* element = (i_ccam_PK*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

/************************************************************/
/*  I : campains to swap                                    */
/*  P : Swaps two campains                                  */
/*  O : 0 -> Swapped                                        */
/*     -1 -> Error                                          */
/************************************************************/
int swap_campain(void* first, void* second){
    ccam_recur tmp;

    if(!first || !second)
        return -1;

    memset(&tmp, 0, sizeof(ccam_recur));

    assign_campain((void*)&tmp, first);
    assign_campain(first, second);
    assign_campain(second, (void*)&tmp);

    return 0;
}

/************************************************************/
/*  I : campains to swap                                    */
/*  P : Swaps two campains                                  */
/*  O : 0 -> Swapped                                        */
/*     -1 -> Error                                          */
/************************************************************/
int swap_campain_index(void* first, void* second){
    i_ccam_PK tmp;

    if(!first || !second)
        return -1;

    memset(&tmp, 0, sizeof(i_ccam_PK));

    assign_campain_index((void*)&tmp, first);
    assign_campain_index(first, second);
    assign_campain_index(second, (void*)&tmp);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Gets the element to the right of the current        */
/*  O : Address of the element to the right                 */
/*          (NULL if current is null)                       */
/************************************************************/
void** campain_right(void* current){
    ccam_recur* currentcam = (ccam_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentcam->right;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Gets the element to the left of the current         */
/*  O : Address of the element to the left                  */
/*          (NULL if current is null)                       */
/************************************************************/
void** campain_left(void* current){
    ccam_recur* currentcam = (ccam_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentcam->left;
}

/************************************************************/
/*  I : record to display                                   */
/*      /                                                   */
/*  P : Displays an algo-compatible record                  */
/*  O : /                                                   */
/************************************************************/
int Rec_campain_list(void *record, void* nullable){
    ccam_recur* tmp = (ccam_recur*)record;

    Rec_campain(&tmp->cam);

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the campain          */
/*  O : /                                                   */
/************************************************************/
char* toString_campain(void* current){
    ccam_recur *tmp = (ccam_recur*)current;

    return tmp->cam.nm_cam;
}

/************************************************************/
/*  I : campain AVL leaf of which to get the height        */
/*  P : Gets the height of the current AVL leaf             */
/*  O : Leaf height                                         */
/************************************************************/
int get_campain_height(void* current){
    ccam_recur *tmp = (ccam_recur*)current;

    return (tmp == NULL ? 0 : tmp->height);
}

/************************************************************/
/*  I : campain AVL leaf of which to set the height        */
/*      New value for the height                            */
/*  P : Sets the height of the current AVL leaf             */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
int set_campain_height(void* current, int value){
    ccam_recur *tmp = (ccam_recur*)current;

    tmp->height = value;

    return 0;
}

/************************************************************/
/*  I : campain AVL leaf to free                           */
/*      /                                                   */
/*  P : Frees the memory for the current campain           */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
void* free_campain(void* campain, void* nullable){
    free(campain);
    return 0;
}

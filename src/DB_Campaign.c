#include "DB_Campaign.h"

/****************************************************************************************/
/*  I : Database to which import the campaigns CSV file                                  */
/*  P : Reads the whole campaigns CSV file and writes it in the Database file            */
/*  O : /                                                                               */
/****************************************************************************************/
void Import_CSV_campaign(dbc *db){
    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	ccam cam;
	FILE *fpi, *fp_lg;

	//open DB files
    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    //open Import file
	fpi = fopen(CSV_cam_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\ncampaigns : importing ...\n");

    //read the first 200 characters in the import file
    fgets(line, 200, fpi);

    //set the DB file pointer to the beginning of the Campains data block
    fseek(db->fp, db->hdr.off_cam, SEEK_SET);

    printf("%lu\n",(unsigned long int)db->hdr.off_cam);

    while (fgets(line, 200, fpi) != NULL)
    {
        //clean the buffer up and set the record type to cam
        memset(&cam, 0, sizeof(ccam));
        strcpy(cam.tp_rec, "cam");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        //read the campaign ID (ptr1) and the campain name (ptr2), then set the campaign ID
        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        fld[strlen(ptr1)]='\0';
        cam.id_cam = atoi(fld);                    if (PRT) printf("%d\n", cam.id_cam);

        //set the campaign name and read the campaign type
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.nm_cam, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        cam.nm_cam[strlen(ptr1)]='\0';

        //set the campaign type and read the campaign date
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.tp_cam, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        cam.tp_cam[strlen(ptr1)]='\0';

        //set the campaign date and read the campaign level denomination
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.dt_cam, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        cam.dt_cam[strlen(ptr1)]='\0';

        //set the campaign level denomination and read the department
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.nm_lev, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        cam.nm_lev[strlen(ptr1)]='\0';

        //set the department and read the sector
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.nm_dep, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        cam.nm_dep[strlen(ptr1)]='\0';

        //set the sector and read the zone
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.nm_sec, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        cam.nm_sec[strlen(ptr1)]='\0';

        //set the zone and read the year
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cam.nm_zon, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cam.nm_cam);
        cam.nm_zon[strlen(ptr1)]='\0';

        //set the year and read the cost
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(fld, ptr1, ptr2-ptr1-1);
        fld[strlen(ptr1)]='\0';
        cam.nr_year = atoi(fld);                   if (PRT) printf("%d\n", cam.id_cam);

        //set the cost
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        fld[strlen(ptr1)]='\0';
        cam.cost = atof(fld);                      if (PRT) printf("%d\n", cam.id_cam);

        fwrite(&cam, 1, sizeof(ccam), db->fp);

        i++;
    }

    db->nr_cam = i;

    fprintf(fp_lg, "campaigns imported : %lu\n", (unsigned long int)db->nr_cam);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\ncampaigns imported : %lu\n\n", (unsigned long int)db->nr_cam);

	return ;
}

/****************************************************************************************/
/*  I : Database from which export the campaigns CSV file                                */
/*  P : Reads the whole campaigns database and exports it in a CSV file                  */
/*  O : /                                                                               */
/****************************************************************************************/
void Export_CSV_campaign(dbc *db){
    uint64_t i;
	ccam cam;
	FILE *fpo, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    printf("\ncampaign : exporting ...\n");
    fpo = fopen("Data_Export/Exp_campaign.csv", "w");
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

    fprintf(fp_lg, "campaign exported : %lu\n", (unsigned long int)db->nr_cam);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\ncampaign exported : %lu\n\n", (unsigned long int)db->nr_cam);

    return;
}

/****************************************************************************************/
/*  I : Database from which import the records in memory                                */
/*  P : Reads the whole campaigns database and loads them in memory                      */
/*  O : /                                                                               */
/****************************************************************************************/
void Load_campaign(dbc *db){
    uint64_t i;
	ccam cam;
	FILE *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    if(db->cam)
        free(db->cam);

    db->cam = (ccam*)calloc(db->nr_cam, sizeof(ccam));

    printf("\ncampaign : loading ...\n");

    fseek(db->fp, db->hdr.off_cam, SEEK_SET);

    for (i=0; i<db->nr_cam; i++)
    {
        memset(&cam, 0, sizeof(ccam));
        fread(&cam, 1, sizeof(ccam), db->fp);

        db->cam[i] = cam;
    }

    fprintf(fp_lg, "campaign loaded into buffer : %lu\n", (unsigned long int)db->nr_cam);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\ncampaign loaded into buffer : %lu\n\n", (unsigned long int)db->nr_cam);

    return;
}

/****************************************************************************************/
/*  I : Database from which print the campaigns                                          */
/*  P : Prints all the campaigns in the memory buffer                                    */
/*  O : /                                                                               */
/****************************************************************************************/
void Print_campaign(dbc *db){
    uint64_t i;

    for (i=0; i<db->nr_cam; i++)
        Rec_campaign(&db->cam[i]);

    return;
}

/****************************************************************************************/
/*  I : campaign record to print                                                        */
/*  P : Prints an campaign record                                                       */
/*  O : /                                                                               */
/****************************************************************************************/
void Rec_campaign(ccam *rec){
    printf("%4d %40s %16s %10s %16s %10s %16s %16s %3d %f\n",
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
/*  I : First campaign to compare                                                        */
/*      Second  campaign to compare                                                      */
/*  P : Compares two campaigns by their PK                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_campaign_PK(void* a, void* b){
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
/*  I : First campaign to compare                                                        */
/*      Second  campaign to compare                                                      */
/*  P : Compares two campaigns by their PK                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_campaign_PK_index(void* a, void* b){
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
/*  I : campaign index element to compare                                                */
/*      PK of the second campaign to compare                                             */
/*  P : Compares two campaigns by their PK                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_campaign_index_int(void* a, void* b){
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
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_campaign_index_slot(void* index, void* offset){
    i_ccam_PK* element = (i_ccam_PK*)index;
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
void** campaign_right(void* current){
    ccam_recur* currentcam = (ccam_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentcam->right;
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
int Rec_campaign_list(void *record, void* nullable){
    ccam_recur* tmp = (ccam_recur*)record;

    Rec_campaign(&tmp->cam);

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the campaign          */
/*  O : /                                                   */
/************************************************************/
char* toString_campaign(void* current){
    ccam_recur *tmp = (ccam_recur*)current;

    return tmp->cam.nm_cam;
}

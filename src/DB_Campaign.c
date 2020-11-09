#include "DB_Campaign.h"

/****************************************************************************************/
/*  I : CSV line to transform to a Campaign                                             */
/*      Buffer in which store the deserialised information                              */
/*  P : Transfrom a CSV line to a Campaign                                              */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVDeserialiseCampaign(char *line, void *record){
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	ccam *cam = (ccam*)record;

	strcpy(cam->tp_rec, "CAM");

    //read the campaign ID (ptr1) and the campain name (ptr2), then set the campaign ID
    ptr1 = strtok(line,";");
    ptr2 = strtok(NULL,";");
    memset(fld, 0, BUF_LEN);
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    cam->id_cam = atoi(fld);

    //set the campaign name and read the campaign type
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cam->nm_cam, ptr1, ptr2-ptr1-1);
    cam->nm_cam[strlen(ptr1)]='\0';

    //set the campaign type and read the campaign date
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cam->tp_cam, ptr1, ptr2-ptr1-1);
    cam->tp_cam[strlen(ptr1)]='\0';

    //set the campaign date and read the campaign level denomination
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cam->dt_cam, ptr1, ptr2-ptr1-1);
    cam->dt_cam[strlen(ptr1)]='\0';

    //set the campaign level denomination and read the department
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cam->nm_lev, ptr1, ptr2-ptr1-1);
    cam->nm_lev[strlen(ptr1)]='\0';

    //set the department and read the sector
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cam->nm_dep, ptr1, ptr2-ptr1-1);
    cam->nm_dep[strlen(ptr1)]='\0';

    //set the sector and read the zone
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cam->nm_sec, ptr1, ptr2-ptr1-1);
    cam->nm_sec[strlen(ptr1)]='\0';

    //set the zone and read the year
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cam->nm_zon, ptr1, ptr2-ptr1-1);
    cam->nm_zon[strlen(ptr1)]='\0';

    //set the year and read the cost
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    cam->nr_year = atoi(fld);

    //set the cost
    ptr1 = ptr2;
    memset(fld, 0, BUF_LEN);
    strncpy(fld, ptr1, strlen(ptr1));
    fld[strlen(ptr1)]='\0';
    cam->cost = atof(fld);

	return 0;
}

/****************************************************************************************/
/*  I : Campaign to format to CSV                                                       */
/*      Buffer in which the final CSV line will be stored                               */
/*  P : Formats a Campaign to a CSV file line                                           */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVFormatCampaign(void* elem, char* finalLine){
    ccam* cam = (ccam*)elem;

    sprintf(finalLine,"%d;%s;%s;%s;%s;%s;%s;%s;%d; %.3f\n",
                cam->id_cam,
                cam->nm_cam,
                cam->tp_cam,
                cam->dt_cam,
                cam->nm_lev,
                cam->nm_dep,
                cam->nm_sec,
                cam->nm_zon,
                cam->nr_year,
                cam->cost);

    return 0;
}

/****************************************************************************************/
/*  I : campaign record to print                                                        */
/*  P : Prints an campaign record                                                       */
/*  O : /                                                                               */
/****************************************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Rec_campaign(void *rec, void* nullable){
    ccam* tmp = (ccam*)rec;

    printf("%4d %40s %16s %10s %16s %10s %16s %16s %3d %f\n",
            tmp->id_cam,
            tmp->nm_cam,
            tmp->tp_cam,
            tmp->dt_cam,
            tmp->nm_lev,
            tmp->nm_dep,
            tmp->nm_sec,
            tmp->nm_zon,
            tmp->nr_year,
            tmp->cost);

    return 0;
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
    ccam *tmp_a = (ccam*)a;
    ccam *tmp_b = (ccam*)b;

    if(tmp_a->id_cam > tmp_b->id_cam)
        return 1;
    else if(tmp_a->id_cam < tmp_b->id_cam)
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
/*  I : campaign index buffer to which copy data                                        */
/*      campaign from which copy data                                                   */
/*  P : Copies all the fields of a campaign to a campaign index buffer                  */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_campaign_index_PK(void* index, void* elem){
    ccam* element = (ccam*)elem;
    i_ccam_PK* i_element = (i_ccam_PK*)index;

    if(!index || !element)
        return -1;

    //copy the data from the campaign to the buffer
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
int assign_campaign_index_slot(void* index, uint32_t* offset){
    i_ccam_PK* element = (i_ccam_PK*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the campaign          */
/*  O : /                                                   */
/************************************************************/
char* toString_campaign(void* current){
    ccam *tmp = (ccam*)current;

    return tmp->nm_cam;
}

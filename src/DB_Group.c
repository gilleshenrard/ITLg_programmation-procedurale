#include "DB_Group.h"

/****************************************************************************************/
/*  I : CSV line to transform to a Group                                                */
/*      Buffer in which store the deserialised information                              */
/*  P : Transfrom a CSV line to a Group                                                 */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVDeserialiseGroup(char *line, void *record){
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	cgrp *grp = (cgrp*)record;

	strcpy(grp->tp_rec, "GRP");

    //read the group ID (ptr1) and the group name (ptr2), then set the group ID
    ptr1 = strtok(line,";");
    ptr2 = strtok(NULL,";");
    memset(fld, 0, BUF_LEN);
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    grp->id_grp = atoi(fld);

    //set the group name and read the group country
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(grp->nm_grp, ptr1, ptr2-ptr1-1);
    grp->nm_grp[strlen(ptr1)]='\0';

    //set the group country and read the group country ID
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(grp->cd_cty, ptr1, ptr2-ptr1-1);
    grp->cd_cty[strlen(ptr1)]='\0';

    //set the group country ID
    ptr1 = ptr2;
    memset(fld, 0, BUF_LEN);
    strncpy(fld, ptr1, strlen(ptr1));
    fld[strlen(ptr1)]='\0';
    grp->id_cty = atoi(fld);

	return 0;
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

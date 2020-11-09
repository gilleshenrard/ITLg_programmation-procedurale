#include "DB_Contact.h"

/****************************************************************************************/
/*  I : CSV line to transform to a Contact                                              */
/*      Buffer in which store the deserialised information                              */
/*  P : Transfrom a CSV line to a Contact                                               */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVDeserialiseContact(char *line, void *record){
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	ccon *con = (ccon*)record;

    //read the campain ID (ptr1) and the company ID (ptr2), then set the campaign ID
    ptr1 = strtok(line,";");
    ptr2 = strtok(NULL,";");
    memset(fld, 0, BUF_LEN);
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    con->id_cam = atoi(fld);

    //set the company ID and read the job ID
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    con->id_cpy = atoi(fld);

    //set the job ID and read the amount of replies
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    con->id_job = atoi(fld);

    //set the amount of replies
    ptr1 = ptr2;
    memset(fld, 0, BUF_LEN);
    strncpy(fld, ptr1, strlen(ptr1)-1);
    fld[strlen(ptr1)]='\0';
    con->nr_rep = atof(fld);

	return 0;
}

/****************************************************************************************/
/*  I : Contact to format to CSV                                                        */
/*      Buffer in which the final CSV line will be stored                               */
/*  P : Formats a Contact to a CSV file line                                            */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVFormatContact(void* elem, char* finalLine){
    ccon* con = (ccon*)elem;

    sprintf(finalLine,"%d;%d;%d;%d\n",
            con->id_cam,
            con->id_cpy,
            con->id_job,
            con->nr_rep);

    return 0;
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

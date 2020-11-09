#include "DB_Industry.h"

/****************************************************************************************/
/*  I : CSV line to transform to an Industry                                            */
/*      Buffer in which store the deserialised information                              */
/*  P : Transfrom a CSV line to an Industry                                             */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVDeserialiseIndustry(char *line, void *record){
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	cind *ind = (cind*)record;

    //read the industry ID (ptr1) and the sector name (ptr2), then set the industry ID
    ptr1 = strtok(line,";");
    ptr2 = strtok(NULL,";");
    memset(fld, 0, BUF_LEN);
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    ind->id_ind = atoi(fld);

    //set the sector name and read the industry name
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(ind->nm_sec, ptr1, ptr2-ptr1-1);
    ind->nm_sec[strlen(ptr1)]='\0';

    //set the industry name
    ptr1 = ptr2;
    memset(fld, 0, BUF_LEN);
    strncpy(ind->nm_ind, ptr1, strlen(ptr1));
    ind->nm_ind[strlen(ptr1)]='\0';

	return 0;
}

/****************************************************************************************/
/*  I : Industry to format to CSV                                                       */
/*      Buffer in which the final CSV line will be stored                               */
/*  P : Formats a Industry to a CSV file line                                           */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVFormatIndustry(void* elem, char* finalLine){
    cind* ind = (cind*)elem;

    sprintf(finalLine,"%d;%s;%s\n",
            ind->id_ind,
            ind->nm_sec,
            ind->nm_ind);

    return 0;
}

/****************************************************************************************/
/*  I : industry record to print                                                        */
/*  P : Prints an industry record                                                       */
/*  O : /                                                                               */
/****************************************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Rec_industry(void *rec, void *nullable){
    cind* tmp = (cind*)rec;

    printf("%4d %32s %4s\n",
           tmp->id_ind,
           tmp->nm_sec,
           tmp->nm_ind );

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/****************************************************************************************/
/*  I : First industry to compare                                                       */
/*      Second  industry to compare                                                     */
/*  P : Compares two industries by their PK                                             */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_industry_PK(void* a, void* b){
    cind *tmp_a = (cind*)a;
    cind *tmp_b = (cind*)b;

    if(tmp_a->id_ind > tmp_b->id_ind)
        return 1;
    else if(tmp_a->id_ind < tmp_b->id_ind)
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
int assign_industry_index_slot(void* index, uint32_t* offset){
    i_cind_PK* element = (i_cind_PK*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

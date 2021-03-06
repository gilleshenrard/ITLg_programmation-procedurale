#include "DB_Company.h"

/****************************************************************************************/
/*  I : CSV line to transform to a Company                                              */
/*      Buffer in which store the deserialised information                              */
/*  P : Transfrom a CSV line to a Company                                               */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVDeserialiseCompany(char *line, void *record)
{
    ccpy* cpy = (ccpy*)record;
    char *ptr1=NULL, *ptr2=NULL;
    char fld[BUF_LEN];

    strcpy(cpy->tp_rec, "CPY");

    //read the company ID (ptr1) and the country ID (ptr2), then set the company ID
    ptr1 = strtok(line,";");
    ptr2 = strtok(NULL,";");
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    cpy->id_cpy = atoi(fld);

    //set the country ID and read the industry ID
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    cpy->id_cty = atoi(fld);

    //set the industry ID and read the group ID
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    cpy->id_ind = atoi(fld);

    //set the group ID and read the company name
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    cpy->id_grp = atoi(fld);

    //set the company name and read the company address
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cpy->nm_cpy, ptr1, ptr2-ptr1-1);
    cpy->nm_cpy[strlen(ptr1)]='\0';

    //set the company address and read the company city
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cpy->nm_adr, ptr1, ptr2-ptr1-1);
    cpy->nm_adr[strlen(ptr1)]='\0';

    //set the company postal code and read the company city name
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cpy->cd_pos, ptr1, ptr2-ptr1-1);
    cpy->cd_pos[strlen(ptr1)]='\0';

    //set the company city name and read the phone number
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cpy->nm_cit, ptr1, ptr2-ptr1-1);
    cpy->nm_cit[strlen(ptr1)]='\0';

    //set the company postal code and read the company website
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cpy->nr_tel, ptr1, ptr2-ptr1-1);
    cpy->nr_tel[strlen(ptr1)]='\0';

    //set the company website and read the company creation date
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cpy->nm_www, ptr1, ptr2-ptr1-1);
    cpy->nm_www[strlen(ptr1)]='\0';

    //set the company creation date
    ptr1 = ptr2;
    strncpy(cpy->dt_cre, ptr1, strlen(ptr1));
    cpy->dt_cre[strlen(ptr1)]='\0';

	return 0;
}

/****************************************************************************************/
/*  I : Company to format to CSV                                                        */
/*      Buffer in which the final CSV line will be stored                               */
/*  P : Formats a Company to a CSV file line                                            */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVFormatCompany(void* elem, char* finalLine){
    ccpy* cpy = (ccpy*)elem;

            sprintf(finalLine,"%d;%d;%d;%d;%s;%s;%s;%s;%s;%s;%s\n",
                cpy->id_cpy,
                cpy->id_cty,
                cpy->id_ind,
                cpy->id_grp,
                cpy->nm_cpy,
                cpy->nm_adr,
                cpy->cd_pos,
                cpy->nm_cit,
                cpy->nr_tel,
                cpy->nm_www,
                cpy->dt_cre);

    return 0;
}

/****************************************************************************************
* Imprime un record Company depuis le buffer
****************************************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Rec_company(void *rec, void *nullable)
{
    ccpy *tmp = (ccpy*)rec;

    printf("%6d %30s %40s %16s \n",
           tmp->id_cpy,
           tmp->nm_cpy,
           tmp->nm_www,
           tmp->cd_pos);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************/
/*  I : First Company to compare                                                        */
/*      Second  Company to compare                                                      */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_company_name(void* a, void* b){
    ccpy *tmp_a = (ccpy*)a;
    ccpy *tmp_b = (ccpy*)b;

    return strcmp(tmp_a->nm_cpy, tmp_b->nm_cpy);
}

/****************************************************************************************/
/*  I : First Company to compare                                                        */
/*      Second  Company to compare                                                      */
/*  P : Compares two countries by their group ID                                        */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_company_grp(void* a, void* b){
    ccpy *tmp_a = (ccpy*)a;
    ccpy *tmp_b = (ccpy*)b;

    if(tmp_a->id_grp > tmp_b->id_grp)
        return 1;
    else if(tmp_a->id_grp < tmp_b->id_grp)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : First Company to compare                                                        */
/*      Second  Company to compare                                                      */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_company_index_name(void* a, void* b){
    i_ccpy_name *tmp_a = (i_ccpy_name*)a;
    i_ccpy_name *tmp_b = (i_ccpy_name*)b;

    return strcmp(tmp_a->nm_cpy, tmp_b->nm_cpy);
}

/****************************************************************************************/
/*  I : Company index element to compare                                                */
/*      Name of the second  Company to compare                                          */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_company_index_char(void* a, void* b){
    i_ccpy_name *tmp_a = (i_ccpy_name*)a;

    return strcmp(tmp_a->nm_cpy, (char*)b);
}

/****************************************************************************************/
/*  I : First Company to compare                                                        */
/*      Second  Company to compare                                                      */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_company_index_grp(void* a, void* b){
    i_ccpy_grp *tmp_a = (i_ccpy_grp*)a;
    i_ccpy_grp *tmp_b = (i_ccpy_grp*)b;

    if(tmp_a->grp_id > tmp_b->grp_id)
        return 1;
    else if(tmp_a->grp_id < tmp_b->grp_id)
        return -1;
    else
        return 0;
}

/****************************************************************************************/
/*  I : Company index element to compare                                                */
/*      Name of the second  Company to compare                                          */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_company_index_int(void* a, void* b){
    i_ccpy_grp *tmp_a = (i_ccpy_grp*)a;
    int * tmp_b = (int*)b;

    if(tmp_a->grp_id > *tmp_b)
        return 1;
    else if(tmp_a->grp_id < *tmp_b)
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
int assign_company_index_nm_slot(void* index, uint32_t* offset){
    i_ccpy_name* element = (i_ccpy_name*)index;
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
int assign_company_index_grp_slot(void* index, uint32_t* offset){
    i_ccpy_grp* element = (i_ccpy_grp*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

/************************************************************/
/*  I : Index element to which copy the data                */
/*      Table element from which copy the data              */
/*  P : Fill an index element with the proper information   */
/*  O : -1 if error                                         */
/*      0 otherwise                                         */
/************************************************************/
int assign_company_index_name(void* index, void* elem){
    ccpy* element = (ccpy*)elem;
    i_ccpy_name* i_element = (i_ccpy_name*)index;

    if(!index || !element)
        return -1;

    //copy the data from the Company to the buffer
    strcpy(i_element->nm_cpy, element->nm_cpy);
    strcpy(i_element->tp_rec, "I_CPYNM");

    return 0;
}

/************************************************************/
/*  I : Index element to which copy the data                */
/*      Table element from which copy the data              */
/*  P : Fill an index element with the proper information   */
/*  O : -1 if error                                         */
/*      0 otherwise                                         */
/************************************************************/
int assign_company_index_grp(void* index, void* elem){
    ccpy* element = (ccpy*)elem;
    i_ccpy_grp* i_element = (i_ccpy_grp*)index;

    if(!index || !element)
        return -1;

    //copy the data from the Company to the buffer
    i_element->grp_id = element->id_grp;
    strcpy(i_element->tp_rec, "I_CPYGR");

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the Company           */
/*  O : /                                                   */
/************************************************************/
char* toString_company(void* current){
    ccpy *tmp = (ccpy*)current;

    return tmp->nm_cpy;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// FILES METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DB_Country.h"

/****************************************************************************************/
/*  I : CSV line to transform to a Country                                              */
/*      Buffer in which store the deserialised information                              */
/*  P : Transfrom a CSV line to a Country                                               */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVDeserialiseCountry(char *line, void *record){
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	ccty *cty = (ccty*)record;

    //read the country ID (ptr1) and the country name (ptr2), then set the country ID
    ptr1 = strtok(line,";");
    ptr2 = strtok(NULL,";");
    memset(fld, 0, BUF_LEN);
    strncpy(fld, ptr1, ptr2-ptr1-1);
    fld[strlen(ptr1)]='\0';
    cty->id_cty = atoi(fld);

    //set the country name and read the country zone
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cty->nm_cty, ptr1, ptr2-ptr1-1);
    cty->nm_cty[strlen(ptr1)]='\0';

    //set the country zone and read the country ISO code
    ptr1 = ptr2;
    ptr2 = strtok(NULL,";");
    strncpy(cty->nm_zon, ptr1, ptr2-ptr1-1);
    cty->nm_zon[strlen(ptr1)]='\0';

    //set the country ISO code
    ptr1 = ptr2;
    strncpy(cty->cd_iso, ptr1, strlen(ptr1));
    cty->cd_iso[strlen(ptr1)]='\0';

	return 0;
}

/****************************************************************************************/
/*  I : Country to format to CSV                                                        */
/*      Buffer in which the final CSV line will be stored                               */
/*  P : Formats a Country to a CSV file line                                            */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int CSVFormatCountry(void* elem, char* finalLine){
    ccty* cty = (ccty*)elem;

        sprintf(finalLine,"%d;%s;%s;%s\n",
                cty->id_cty,
                cty->nm_cty,
                cty->nm_zon,
                cty->cd_iso);

    return 0;
}

/****************************************************************************************
* Imprime un record Country depuis le buffer
****************************************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Rec_Country(void *rec, void* nullable){
    ccty* tmp = (ccty*)rec;

    printf("%3d %20s %20s %2s \n",
           tmp->id_cty,
           tmp->nm_zon,
           tmp->nm_cty,
           tmp->cd_iso );

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/****************************************************************************************/
/*  I : First country to compare                                                        */
/*      Second  country to compare                                                      */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_country_name(void* a, void* b){
    ccty *tmp_a = (ccty*)a;
    ccty *tmp_b = (ccty*)b;

    return strcmp(tmp_a->nm_cty, tmp_b->nm_cty);
}

/****************************************************************************************/
/*  I : First country to compare                                                        */
/*      Second  country to compare                                                      */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_country_index_name(void* a, void* b){
    i_ccty_name *tmp_a = (i_ccty_name*)a;
    i_ccty_name *tmp_b = (i_ccty_name*)b;

    return strcmp(tmp_a->nm_cty, tmp_b->nm_cty);
}

/****************************************************************************************/
/*  I : Country index buffer to which copy data                                         */
/*      Country from which copy data                                                    */
/*  P : Copies all the fields of a country to a country index buffer                    */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_country_index_name(void* index, void* elem){
    ccty* element = (ccty*)elem;
    i_ccty_name* i_element = (i_ccty_name*)index;

    if(!index || !element)
        return -1;

    //copy the data from the country to the buffer
    strcpy(i_element->nm_cty, element->nm_cty);
    strcpy(i_element->tp_rec, "I_CTYNM");

    return 0;
}

/****************************************************************************************/
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_country_index_slot(void* index, uint32_t* offset){
    i_ccty_name* element = (i_ccty_name*)index;
    long* slot = (long*)offset;

    if(!index || !offset)
        return -1;

    element->slot = *slot;

    return 0;
}

#include "../lib/DB_Country.h"

/****************************************************************************************
* Chargement du fichier DB_Country.csv dans la database
*
* PRT controle l'affichage de test (a enlever)
*
****************************************************************************************/
void Import_CSV_Country(dbc *db)
{
    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	ccty cty;
	FILE *fpi, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

	fpi = fopen("Data_Import/DB_Country.csv", "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nCountry : importing ...\n");

    fgets(line, 200, fpi);

    fseek(db->fp, db->hdr.off_cty, SEEK_SET);

    printf("%08X\n",db->hdr.off_cty);

    while (fgets(line, 200, fpi) != NULL)
    {
        memset(&cty, 0, sizeof(ccty));
        strcpy(cty.tp_rec, "CTY");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cty.id_cty = atoi(fld);                    if (PRT) printf("%d\n", cty.id_cty);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cty.nm_cty, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cty.nm_cty);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cty.nm_zon, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cty.nm_zon);
        ptr1 = ptr2;
        strncpy(cty.cd_iso, ptr1, strlen(ptr1)-1); if (PRT) printf("%s\n", cty.cd_iso);

        fwrite(&cty, 1, sizeof(ccty), db->fp);

        i++;
    }

    db->nr_cty = i;

    fprintf(fp_lg, "Country imported : %d \n", db->nr_cty);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nCountry imported : %d \n\n", db->nr_cty);

	return ;
}

/****************************************************************************************
* Exporte table Country dans un fichier .csv
****************************************************************************************/
void Export_CSV_Country(dbc *db)
{
    int i;
	ccty cty;
	FILE *fpo, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    printf("\nCountry : exporting ...\n");
    fpo = fopen("Data_Export/Exp_Country.csv", "w");
    fprintf(fpo,"Id;Nm_Cty;Nm_Zon;Cd_Iso\n");

    fseek(db->fp, db->hdr.off_cty, SEEK_SET);

    for (i=0; i<db->nr_cty; i++)
    {
        memset(&cty, 0, sizeof(ccty));
        fread(&cty, 1, sizeof(ccty), db->fp);

        fprintf(fpo,"%d;%s;%s;%s\n",
                cty.id_cty,
                cty.nm_cty,
                cty.nm_zon,
                cty.cd_iso);
    }

    fprintf(fp_lg, "Country exported : %d \n", db->nr_cty);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\nCountry exported : %d \n\n", db->nr_cty);

    return;
}

/****************************************************************************************
* Charge table Country dans le buffer
****************************************************************************************/
void Load_Country(dbc *db)
{
    int i;
	ccty cty;
	FILE *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    printf("\nCountry : loading ...\n");

    fseek(db->fp, db->hdr.off_cty, SEEK_SET);

    for (i=1; i<=db->nr_cty; i++)
    {
        memset(&cty, 0, sizeof(ccty));
        fread(&cty, 1, sizeof(ccty), db->fp);

        db->cty[i] = cty;
    }

    fprintf(fp_lg, "Country loaded into buffer : %d \n", db->nr_cty);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nCountry loaded into buffer : %d \n\n", db->nr_cty);

    return;
}

/****************************************************************************************
* Liste la table Country depuis le buffer
****************************************************************************************/
void Print_Country(dbc *db)
{
    int i;

    for (i=1; i<=db->nr_cty; i++)
        Rec_Country(&db->cty[i]);

    return;
}

/****************************************************************************************
* Imprime un record Country depuis le buffer
****************************************************************************************/
void Rec_Country(ccty *rec)
{
    printf("%3d %20s %20s %2s \n",
           rec->id_cty,
           rec->nm_zon,
           rec->nm_cty,
           rec->cd_iso );

    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************/
/*  I : /                                                                               */
/*  P : Allocates memory for a country and sets its height to 1 (leaf for AVL)          */
/*  O : Country created if OK                                                           */
/*      NULL if error                                                                   */
/****************************************************************************************/
void* allocate_country(void){
    ccty_recur *tmp=NULL;

    //memory allocation for the new element (calloc to initialize with all 0)
    tmp = calloc(1, sizeof(ccty_recur));
    if(tmp) tmp->height = 1;

    return tmp;
}

/****************************************************************************************/
/*  I : First country to compare                                                        */
/*      Second  country to compare                                                      */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_country_name(void* a, void* b){
    ccty_recur *tmp_a = (ccty_recur*)a;
    ccty_recur *tmp_b = (ccty_recur*)b;

    return strcmp(tmp_a->cty.nm_cty, tmp_b->cty.nm_cty);
}

/****************************************************************************************/
/*  I : First country to compare                                                        */
/*      Name of the second  country to compare                                          */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_country_name_char(void* a, void* b){
    ccty_recur *tmp_a = (ccty_recur*)a;

    return strcmp(tmp_a->cty.nm_cty, (char*)b);
}

/****************************************************************************************/
/*  I : Country to which copy data                                                      */
/*      Country from which copy data                                                    */
/*  P : Copies all the fields of countries from new to old                              */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_country(void* oldelem, void* newelem){
    ccty_recur* oldTuple = (ccty_recur*)oldelem;
    ccty_recur* newTuple = (ccty_recur*)newelem;
    ccty_recur *saveRight = NULL, *saveLeft=NULL;

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
/*  I : Country index buffer to which copy data                                         */
/*      Country from which copy data                                                    */
/*  P : Copies all the fields of a country to a country index buffer                    */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_country_index_slot(void* elem, void* offset){
    i_ccty_name* i_element = (i_ccty_name*)elem;
    long* slot = (long*)offset;

    if(!elem || !offset)
        return -1;

    //assign the slot to the index element
    i_element->slot = *slot;

    return 0;
}

/************************************************************/
/*  I : Countries to swap                                   */
/*  P : Swaps two countries                                 */
/*  O : 0 -> Swapped                                        */
/*     -1 -> Error                                          */
/************************************************************/
int swap_country(void* first, void* second){
    ccty_recur tmp;

    if(!first || !second)
        return -1;

    memset(&tmp, 0, sizeof(ccty_recur));

    assign_country((void*)&tmp, first);
    assign_country(first, second);
    assign_country(second, (void*)&tmp);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Gets the element to the right of the current        */
/*  O : Address of the element to the right                 */
/*          (NULL if current is null)                       */
/************************************************************/
void** country_right(void* current){
    ccty_recur* currentCty = (ccty_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentCty->right;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Gets the element to the left of the current         */
/*  O : Address of the element to the left                  */
/*          (NULL if current is null)                       */
/************************************************************/
void** country_left(void* current){
    ccty_recur* currentCty = (ccty_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentCty->left;
}

/************************************************************/
/*  I : record to display                                   */
/*      /                                                   */
/*  P : Displays an algo-compatible record                  */
/*  O : /                                                   */
/************************************************************/
int Rec_Country_list(void *record, void* nullable){
    ccty_recur* tmp = (ccty_recur*)record;

    Rec_Country(&tmp->cty);

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the country           */
/*  O : /                                                   */
/************************************************************/
char* toString_Country(void* current){
    ccty_recur *tmp = (ccty_recur*)current;

    return tmp->cty.nm_cty;
}

/************************************************************/
/*  I : Country AVL leaf of which to get the height         */
/*  P : Gets the height of the current AVL leaf             */
/*  O : Leaf height                                         */
/************************************************************/
int get_country_height(void* current){
    ccty_recur *tmp = (ccty_recur*)current;


    return (tmp == NULL ? 0 : tmp->height);
}

/************************************************************/
/*  I : Country AVL leaf of which to set the height         */
/*      New value for the height                            */
/*  P : Sets the height of the current AVL leaf             */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
int set_country_height(void* current, int value){
    ccty_recur *tmp = (ccty_recur*)current;

    tmp->height = value;

    return 0;
}

/************************************************************/
/*  I : Country AVL leaf to free                            */
/*      /                                                   */
/*  P : Frees the memory for the current country            */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
void* free_country(void* country, void* nullable){
    free(country);
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// FILES METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************************/
/*  I : database in which create the index                                          */
/*      metadata of the data structure to be indexed                                */
/*      number of slots to create                                                   */
/*      record type of the index                                                    */
/*  P : Creates the slots for the requested index at the end of the database        */
/*  O :  0 if OK                                                                    */
/*      -1 otherwise                                                                */
/************************************************************************************/
long create_country_index_file(dbc* db, t_algo_meta* index, int (*assign_slot)(void*, void*), int buffersize, long* root_off){
    void* index_cty_name = NULL, *buffer = NULL;
    FILE *fp_lg=NULL;
    int i=0;
    long root=0, tmp=0, ret=0;

    //open the files and position the pointers at the end
    db->fp = fopen(DB_file, "r+b");
    fp_lg = fopen(log_file, "a");

    if(!db->fp || !fp_lg)
        return -1;

    //allocate the memory for the full size buffer and set an iterator pointer to it
    index->structure = calloc(db->nr_cty, index->elementsize);
    index_cty_name = index->structure;

    buffer = calloc(1, buffersize);

    //read the country database sequentially and fill the buffer with it
    fseek(db->fp, db->hdr.off_cty, SEEK_SET);
    for(i=0 ; i<db->nr_cty ; i++, index_cty_name+=index->elementsize){
        tmp = ftell(db->fp);
        (*assign_slot)(index_cty_name, &tmp);
        fread(buffer, buffersize, 1, db->fp);
        (*index->doCopy)(index_cty_name, buffer);
        memset(buffer, 0, buffersize);
    }

    //sort the buffer
    quickSort(index, 0, db->nr_cty);

    //save the index offset in the header
    fseek(db->fp, 0, SEEK_END);
    ret = ftell(db->fp);

    //sequentially create all index slots
    index_cty_name = index->structure;
    for(i=0 ; i < db->nr_cty ; i++, index_cty_name+=index->elementsize){
        fwrite(index_cty_name, index->elementsize, 1, db->fp);
    }

    root = index_tree(db->fp, ret, db->nr_cty, index);

    //write the new header values to disk
    db->hdr.db_size += index->elementsize*db->nr_cty;
    db->hdr.i_cty_name = root;
    fseek(db->fp, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, db->fp);

    //add a log entry after the creation
    fprintf(fp_lg, "Index %s created... %d records added\n", "I_CTY_NM", i);

    free(index->structure);
    fclose(db->fp);
    fclose(fp_lg);

    return ret;
}



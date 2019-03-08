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
	FILE *fpi, *fp_db, *fp_lg;

    fp_db = fopen("Data_DB_Comp\\DB_Comp.dat", "rb+");
    fp_lg = fopen("Data_DB_Comp\\DB_Comp.log", "a");

	fpi = fopen("Data_Import/DB_Country.csv", "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nCountry : importing ...\n");

    fgets(line, 200, fpi);

    fseek(fp_db, db->hdr.off_cty, SEEK_SET);

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

        fwrite(&cty, 1, sizeof(ccty), fp_db);

        i++;
    }

    db->nr_cty = i;

    fprintf(fp_lg, "Country imported : %d \n", db->nr_cty);

    fclose(fp_db);
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
	FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("Data_DB_Comp\\DB_Comp.dat", "rb+");
    fp_lg = fopen("Data_DB_Comp\\DB_Comp.log", "a");

    printf("\nCountry : exporting ...\n");
    fpo = fopen("Data_Export/Exp_Country.csv", "w");
    fprintf(fpo,"Id;Nm_Cty;Nm_Zon;Cd_Iso\n");

    fseek(fp_db, db->hdr.off_cty, SEEK_SET);

    for (i=0; i<db->nr_cty; i++)
    {
        memset(&cty, 0, sizeof(ccty));
        fread(&cty, 1, sizeof(ccty), fp_db);

        fprintf(fpo,"%d;%s;%s;%s\n",
                cty.id_cty,
                cty.nm_cty,
                cty.nm_zon,
                cty.cd_iso);
    }

    fprintf(fp_lg, "Country exported : %d \n", db->nr_cty);

    fclose(fp_db);
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
	FILE *fp_db, *fp_lg;

    fp_db = fopen("Data_DB_Comp\\DB_Comp.dat", "rb+");
    fp_lg = fopen("Data_DB_Comp\\DB_Comp.log", "a");

    printf("\nCountry : loading ...\n");

    fseek(fp_db, db->hdr.off_cty, SEEK_SET);

    for (i=1; i<=db->nr_cty; i++)
    {
        memset(&cty, 0, sizeof(ccty));
        fread(&cty, 1, sizeof(ccty), fp_db);

        db->cty[i] = cty;
    }

    fprintf(fp_lg, "Country loaded into buffer : %d \n", db->nr_cty);

    fclose(fp_db);
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
void* allocate_country(void* country){
    ccty_recur *tmp = NULL;

    //memory allocation for the new element (calloc to initialize with all 0)
    country = calloc(1, sizeof(ccty_recur));
    tmp=(ccty_recur*)country;
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
/*
    ccty_recur *tmp = (ccty_recur*)country;
    printf("Freeing %s\n", tmp->cty.nm_cty);
*/
    free(country);

    return 0;
}

/************************************************************/
/*  I : Country AVL leaf to check                           */
/*  P : Checks if the country pointer is NULL               */
/*  O : 1 if NULL                                           */
/*      0 otherwise                                         */
/************************************************************/
int is_country_null(void* country){
    return country == NULL;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// FILES METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/************************************************************/
/*  I : File pointer to the country DB file                 */
/*  P : Generates a binary file with a header and a few     */
/*          (SZ_CITY) records, all available and linked     */
/*  O :  0 if OK                                            */
/*      -1 otherwise                                        */
/************************************************************/
int generate_country_file(FILE* fp, char* filename){
    hder_cty header_cty = {0};
    ccty_file tmp;
    char filename_final[28] = "Data_DB_Comp\\";

    //generate the file name with the relative path, and open the file
    strcat(filename_final, filename);
    fp = fopen(filename_final, "wb");
    if(fp){
        //fill and write the header
        strcpy(header_cty.db_name, filename);
        header_cty.db_size = (SZ_CTY * sizeof(ccty_file)) + sizeof(hder_cty);
        header_cty.PTO = -1;  //no record yet
        fwrite(&header_cty, sizeof(hder_cty), 1, fp);

        //generate as much as countries as specified in SZ_CTY
        //      make each one point to the next (all records are available)
        memset(&tmp, 0, sizeof(ccty_file));
        for(int i=0 ; i<SZ_CTY-1 ; i++){
            tmp.right++;
            fwrite(&tmp, sizeof(ccty_file), 1, fp);
        }
        //write the last one and indicate there is no next record free (end of file)
        tmp.right = -1;
        fwrite(&tmp, sizeof(ccty_file), 1, fp);

        return 0;
    }

    return -1;
}

/************************************************************/
/*  I : File pointer to the current DB                      */
/*  P : Indicates if there is any record in the file        */
/*  O : 1 if no record                                      */
/*      0 otherwise                                         */
/************************************************************/
int is_country_fileEmpty(void* fp){
    FILE* file = (FILE*)fp;
    hder_cty header = {0};

    fseek(file, 0, SEEK_SET);
    fread(&header, sizeof(hder_cty), 1, file);

    return header.PTO == -1;
}

void* generate_country_record(void* fp){
    FILE* file = (FILE*)fp;
    hder_cty tmp = {0};

    fseek(fp, 0, SEEK_SET);
    fread(&tmp, sizeof(hder_cty), 1, file);
}

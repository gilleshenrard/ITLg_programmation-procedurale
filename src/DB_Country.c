#include "DB_Country.h"

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

	fpi = fopen(CSV_cty_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nCountry : importing ...\n");

    fgets(line, 200, fpi);

    fseek(db->fp, db->hdr.off_cty, SEEK_SET);

    printf("%08lX\n",db->hdr.off_cty);

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

    fprintf(fp_lg, "Country imported : %lu\n", (unsigned long int)db->nr_cty);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nCountry imported : %lu\n\n", (unsigned long int)db->nr_cty);

	return ;
}

/****************************************************************************************
* Exporte table Country dans un fichier .csv
****************************************************************************************/
void Export_CSV_Country(dbc *db)
{
    uint64_t i;
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

    fprintf(fp_lg, "Country exported : %lu\n", (unsigned long int)db->nr_cty);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\nCountry exported : %lu\n\n", (unsigned long int)db->nr_cty);

    return;
}

/****************************************************************************************
* Charge table Country dans le buffer
****************************************************************************************/
void Load_Country(dbc *db)
{
    uint64_t i;
	ccty cty;
	FILE *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    if(db->cty)
        free(db->cty);

    db->cty = (ccty*)calloc(db->nr_cty, sizeof(ccty));

    printf("\nCountry : loading ...\n");

    fseek(db->fp, db->hdr.off_cty, SEEK_SET);

    for (i=0; i<db->nr_cty; i++)
    {
        memset(&cty, 0, sizeof(ccty));
        fread(&cty, 1, sizeof(ccty), db->fp);

        db->cty[i] = cty;
    }

    fprintf(fp_lg, "Country loaded into buffer : %lu\n", (unsigned long int)db->nr_cty);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nCountry loaded into buffer : %lu\n\n", (unsigned long int)db->nr_cty);

    return;
}

/****************************************************************************************
* Liste la table Country depuis le buffer
****************************************************************************************/
void Print_Country(dbc *db)
{
    uint64_t i;

    for (i=0; i<db->nr_cty; i++)
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
/*  I : record to display                                   */
/*      /                                                   */
/*  P : Displays an algo-compatible record                  */
/*  O : /                                                   */
/************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// FILES METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

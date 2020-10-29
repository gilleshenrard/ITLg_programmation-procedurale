#include "DB_Company.h"

/****************************************************************************************
* Chargement du fichier DB_Company.csv dans la database
*
* PRT controle l'affichage de test (a enlever)
*
****************************************************************************************/
void Import_CSV_company(dbc *db)
{
    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	ccpy cpy;
	FILE *fpi, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

	fpi = fopen(CSV_cpy_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nCompany : importing ...\n");

    fgets(line, 200, fpi);

    fseek(db->fp, db->hdr.off_cpy, SEEK_SET);

    printf("%08lX\n",db->hdr.off_cpy);

    while (fgets(line, 200, fpi) != NULL)
    {
        memset(&cpy, 0, sizeof(ccpy));
        strcpy(cpy.tp_rec, "cpy");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cpy.id_cpy = atoi(fld);                    if (PRT) printf("%d\n", cpy.id_cpy);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cpy.id_cty = atoi(fld);                    if (PRT) printf("%d\n", cpy.id_cty);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cpy.id_ind = atoi(fld);                    if (PRT) printf("%d\n", cpy.id_ind);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cpy.id_grp = atoi(fld);                    if (PRT) printf("%d\n", cpy.id_grp);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cpy.nm_cpy, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cpy.nm_cpy);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cpy.nm_adr, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cpy.nm_adr);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cpy.nm_cit, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cpy.nm_cit);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cpy.cd_pos, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cpy.cd_pos);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cpy.nr_tel, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cpy.nr_tel);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cpy.nm_www, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", cpy.nm_www);
        ptr1 = ptr2;
        strncpy(cpy.dt_cre, ptr1, strlen(ptr1)-1); if (PRT) printf("%s\n", cpy.dt_cre);

        fwrite(&cpy, 1, sizeof(ccpy), db->fp);

        i++;
    }

    db->nr_cpy = i;

    fprintf(fp_lg, "Company imported : %lu\n", (unsigned long int)db->nr_cpy);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nCompany imported : %lu\n\n", (unsigned long int)db->nr_cpy);

	return ;
}

/****************************************************************************************
* Exporte table Company dans un fichier .csv
****************************************************************************************/
void Export_CSV_company(dbc *db)
{
    uint64_t i;
	ccpy cpy;
	FILE *fpo, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    printf("\nCompany : exporting ...\n");
    fpo = fopen("Data_Export/Exp_Company.csv", "w");
    fprintf(fpo,"Id;Nm_cpy;Nm_Zon;Cd_Iso\n");

    fseek(db->fp, db->hdr.off_cpy, SEEK_SET);

    for (i=0; i<db->nr_cpy; i++)
    {
        memset(&cpy, 0, sizeof(ccpy));
        fread(&cpy, 1, sizeof(ccpy), db->fp);

        fprintf(fpo,"%d;%d;%d;%d;%s;%s;%s%s;%s;%s;%s\n",
                cpy.id_cpy,
                cpy.id_cty,
                cpy.id_ind,
                cpy.id_grp,
                cpy.nm_cpy,
                cpy.nm_adr,
                cpy.nm_cit,
                cpy.cd_pos,
                cpy.nr_tel,
                cpy.nm_www,
                cpy.dt_cre);
    }

    fprintf(fp_lg, "Company exported : %lu\n", (unsigned long int)db->nr_cpy);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    printf("\nCompany exported : %lu\n\n", (unsigned long int)db->nr_cpy);

    return;
}

/****************************************************************************************
* Charge table Company dans le buffer
****************************************************************************************/
void Load_company(dbc *db)
{
    uint64_t i;
	ccpy cpy;
	FILE *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    if(db->cpy)
        free(db->cpy);

    db->cpy = (ccpy*)calloc(db->nr_cpy, sizeof(ccpy));

    printf("\nCompany : loading ...\n");

    fseek(db->fp, db->hdr.off_cpy, SEEK_SET);

    for (i=0; i<db->nr_cpy; i++)
    {
        memset(&cpy, 0, sizeof(ccpy));
        fread(&cpy, 1, sizeof(ccpy), db->fp);

        db->cpy[i] = cpy;
    }

    fprintf(fp_lg, "Company loaded into buffer : %lu\n", (unsigned long int)db->nr_cpy);

    fclose(db->fp);
    fclose(fp_lg);

    printf("\nCompany loaded into buffer : %lu\n\n", (unsigned long int)db->nr_cpy);

    return;
}

/****************************************************************************************
* Liste la table Company depuis le buffer
****************************************************************************************/
void Print_company(dbc *db)
{
    uint64_t i;

    for (i=0; i<db->nr_cpy; i++)
        Rec_company(&db->cpy[i]);

    return;
}

/****************************************************************************************
* Imprime un record Company depuis le buffer
****************************************************************************************/
void Rec_company(ccpy *rec)
{
    printf("%6d %30s %40s %16s \n",
           rec->id_cpy,
           rec->nm_cpy,
           rec->nm_www,
           rec->cd_pos);

    return;
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
    ccpy_recur *tmp_a = (ccpy_recur*)a;
    ccpy_recur *tmp_b = (ccpy_recur*)b;

    return strcmp(tmp_a->cpy.nm_cpy, tmp_b->cpy.nm_cpy);
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
    ccpy_recur *tmp_a = (ccpy_recur*)a;
    ccpy_recur *tmp_b = (ccpy_recur*)b;

    if(tmp_a->cpy.id_grp > tmp_b->cpy.id_grp)
        return 1;
    else if(tmp_a->cpy.id_grp < tmp_b->cpy.id_grp)
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
/*  I : index element to which assign the slot                                          */
/*      slot (file offset) in which the base data element is                            */
/*  P : assigns a slot (file offset) to an index element                                */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_company_index_slot(void* index, void* offset){
    i_ccpy_name* element = (i_ccpy_name*)index;
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
void** company_right(void* current){
    ccpy_recur* currentcpy = (ccpy_recur*)current;

    if(!current)
        return NULL;

    return (void**)&currentcpy->right;
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
int Rec_company_list(void *record, void* nullable){
    ccpy_recur* tmp = (ccpy_recur*)record;

    Rec_company(&tmp->cpy);

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the Company           */
/*  O : /                                                   */
/************************************************************/
char* toString_company(void* current){
    ccpy_recur *tmp = (ccpy_recur*)current;

    return tmp->cpy.nm_cpy;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// FILES METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

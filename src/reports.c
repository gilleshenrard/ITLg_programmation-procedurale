#include "../lib/reports.h"

/****************************************************************************************
* Imprime un record Screen Report depuis le buffer
****************************************************************************************/
void Rec_scr_report(cscr *rec)
{
    printf("%6d %6d %6d\n",
           rec->id_job,
           rec->id_cam,
           rec->nr_rep);

    return;
}

/****************************************************************************************/
/*  I : /                                                                               */
/*  P : Allocates memory for a Screen Report and sets its height to 1 (leaf for AVL)          */
/*  O : Screen Report created if OK                                                           */
/*      NULL if error                                                                   */
/****************************************************************************************/
void* allocate_scr_report(void){
    cscr_recur *tmp=NULL;

    //memory allocation for the new element (calloc to initialize with all 0)
    tmp = calloc(1, sizeof(cscr_recur));
    if(tmp) tmp->height = 1;

    return tmp;
}

/****************************************************************************************/
/*  I : First Screen Report to compare                                                        */
/*      Second  Screen Report to compare                                                      */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_scr_report_type(void* a, void* b){
    cscr_recur *tmp_a = (cscr_recur*)a;
    cscr_recur *tmp_b = (cscr_recur*)b;

    return strcmp(tmp_a->rep.cam_tp, tmp_b->rep.cam_tp);
}

/****************************************************************************************/
/*  I : First scr_report to compare                                                        */
/*      Name of the second  Screen Report to compare                                          */
/*  P : Compares two countries by their names                                           */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_scr_report_type_char(void* a, void* b){
    cscr_recur *tmp_a = (cscr_recur*)a;

    return strcmp(tmp_a->rep.cam_tp, (char*)b);
}

/****************************************************************************************/
/*  I : Screen Report to which copy data                                                      */
/*      Screen Report from which copy data                                                    */
/*  P : Copies all the fields of countries from new to old                              */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int copy_scr_report(void* oldelem, void* newelem){
    cscr_recur* oldTuple = (cscr_recur*)oldelem;
    cscr_recur* newTuple = (cscr_recur*)newelem;
    cscr_recur *saveRight = NULL, *saveLeft=NULL;

    if(!oldelem || !newelem)
        return -1;

    //save the pointer values of the old Screen Report
    saveRight = oldTuple->right;
    saveLeft = oldTuple->left;

    //copy the data from the new Screen Report to the old one
    *oldTuple = *newTuple;

    //restore the pointer values
    oldTuple->right = saveRight;
    oldTuple->left = saveLeft;

    return 0;
}

/****************************************************************************************/
/*  I : Screen Report to which copy data                                                */
/*      Screen Report from which copy data                                              */
/*  P : Copies all the fields of countries from new to old                              */
/*  O :  0 if OK                                                                        */
/*      -1 otherwise                                                                    */
/****************************************************************************************/
int assign_scr_report(void* oldelem, void* newelem){
    cscr_recur* oldTuple = (cscr_recur*)oldelem;
    ccon* newTuple = (ccon*)newelem;
    cscr_recur *saveRight = NULL, *saveLeft=NULL;

    if(!oldelem || !newelem)
        return -1;

    //save the pointer values of the old Screen Report
    saveRight = oldTuple->right;
    saveLeft = oldTuple->left;

    //copy the data from the new Screen Report to the old one
    oldTuple->rep.id_cam = newTuple->id_cam;
    oldTuple->rep.id_job = newTuple->id_job;
    oldTuple->rep.nr_rep = newTuple->nr_rep;

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
int swap_scr_report(void* first, void* second){
    cscr_recur tmp;

    if(!first || !second)
        return -1;

    memset(&tmp, 0, sizeof(cscr_recur));

    copy_scr_report((void*)&tmp, first);
    copy_scr_report(first, second);
    copy_scr_report(second, (void*)&tmp);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Gets the element to the right of the current        */
/*  O : Address of the element to the right                 */
/*          (NULL if current is null)                       */
/************************************************************/
void** scr_report_right(void* current){
    cscr_recur* currentCty = (cscr_recur*)current;

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
void** scr_report_left(void* current){
    cscr_recur* currentCty = (cscr_recur*)current;

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
int Rec_scr_report_list(void *record, void* nullable){
    cscr_recur* tmp = (cscr_recur*)record;

    Rec_scr_report(&tmp->rep);

    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the Screen Report           */
/*  O : /                                                   */
/************************************************************/
char* toString_scr_report(void* current){
    cscr_recur *tmp = (cscr_recur*)current;

    return tmp->rep.cam_nm;
}

/************************************************************/
/*  I : Screen Report AVL leaf of which to get the height         */
/*  P : Gets the height of the current AVL leaf             */
/*  O : Leaf height                                         */
/************************************************************/
int get_scr_report_height(void* current){
    cscr_recur *tmp = (cscr_recur*)current;


    return (tmp == NULL ? 0 : tmp->height);
}

/************************************************************/
/*  I : Screen Report AVL leaf of which to set the height         */
/*      New value for the height                            */
/*  P : Sets the height of the current AVL leaf             */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
int set_scr_report_height(void* current, int value){
    cscr_recur *tmp = (cscr_recur*)current;

    tmp->height = value;

    return 0;
}

/************************************************************/
/*  I : Screen Report AVL leaf to free                            */
/*      /                                                   */
/*  P : Frees the memory for the current Screen Report            */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
void* free_scr_report(void* report, void* nullable){
    free(report);
    return 0;
}

/****************************************************************************************/
/*  I : Database from which print the screen report                                     */
/*  P : Creates and prints a report                                                     */
/*          report : Contacts for a company, sorted by campaign type                    */
/*  O : -1 if not found                                                                 */
/*       0 otherwise                                                                    */
/****************************************************************************************/
int print_screen_report(dbc* db, char* nm_cpy){
    t_algo_meta index_cpy = {0};
    t_algo_meta list_cpy = {NULL, 0, sizeof(ccpy_recur), compare_company_name, swap_company, assign_company, NULL, NULL, NULL, company_right, company_left};
    t_algo_meta index_con = {0};
    t_algo_meta list_con = {NULL, 0, sizeof(ccon_recur), compare_contact_cpy, swap_contact, assign_contact, NULL, NULL, NULL, contact_right, contact_left};
    int choix = 0;
    char buffer[6] = {0};
    ccpy_recur* cpy_buffer = NULL, *next=NULL;

    //finish preparing the metadata structures
    index_cpy.elementsize = sizeof(i_ccpy_name);
    index_cpy.doCompare = compare_company_index_char;
    index_con.elementsize = sizeof(i_ccon_cpy);
    index_con.doCompare = compare_contact_index_int;

    // open the DB
    db->fp = fopen(DB_file, "rb");
    if(!db->fp){
        fprintf(stderr, "\n\nErreur a l'ouverture de la DB\n\n");
        return -1;
    }

    //search for all occurrences of the company name (creates a linked list)
    searchall_index(db->fp, db->hdr.i_cpy_name, nm_cpy, &index_cpy, &list_cpy, sizeof(ccpy));
    cpy_buffer=list_cpy.structure;

    //if not found, error
    if(!list_cpy.nbelements){
        fprintf(stderr, "\n\n%s : non-trouve\n\n", nm_cpy);
        return -1;
    }

    //if more than one occurrence, make the user choose which one
    if(list_cpy.nbelements > 1){
        printf("\nVeuillez choisir pour quelle compagnie afficher le rapport : (1 a %ld)\n\n", list_cpy.nbelements);
        foreachList(&list_cpy, NULL, Rec_company_list);

        printf("\nChoix : ");
        fflush(stdin);
        fgets(buffer, 6, stdin);
        choix = atoi(buffer);

        next = list_cpy.structure;
        for(int i=1 ; i<choix ; i++){
            next = *company_right(next);
        }
        cpy_buffer = (ccpy_recur*)next;
    }

    searchall_index(db->fp, db->hdr.i_con_cpy, &cpy_buffer->cpy.id_cpy, &index_con, &list_con, sizeof(ccon));
    foreachList(&list_con, NULL, Rec_contact_list);

    while(list_cpy.structure)
        popListTop(&list_cpy);

    while(list_con.structure)
        popListTop(&list_con);

    fclose(db->fp);

    return 0;
}

#include "reports.h"

/****************************************************************************************
* Imprime un record Screen Report depuis le buffer
****************************************************************************************/
void Rec_scr_report(cscr *rec)
{
    printf("%6d %6d %6d %28s %28s\n",
           rec->id_job,
           rec->id_cam,
           rec->nr_rep,
           rec->cam_nm,
           rec->cam_tp);

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
/*  I : First scr_report to compare                                                     */
/*      Name of the second  Screen Report to compare                                    */
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
/*  I : Screen Report to which copy data                                                */
/*      Screen Report from which copy data                                              */
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
/*      Company name to find                                                            */
/*  P : Creates and prints a report                                                     */
/*          report : Contacts for a company, sorted by campaign type                    */
/*  O : -1 if not found                                                                 */
/*       0 otherwise                                                                    */
/****************************************************************************************/
int print_screen_report(dbc* db, char* nm_cpy){
    //metadata for all the lists created
    t_algo_meta list_cpy = {NULL, 0, sizeof(ccpy_recur), compare_company_name, swap_company, assign_company, NULL, NULL, NULL, company_right, company_left};
    t_algo_meta list_con = {NULL, 0, sizeof(ccon_recur), compare_contact_cpy, swap_contact, assign_contact, NULL, NULL, NULL, contact_right, contact_left};
    t_algo_meta list_rep = {NULL, 0, sizeof(cscr_recur), compare_scr_report_type, swap_scr_report, assign_scr_report, NULL, NULL, NULL, scr_report_right, scr_report_left};
    //metadata for all the indexes used
    t_algo_meta index_cpy = {0};
    t_algo_meta index_con = {0};
    t_algo_meta index_cam = {0};

    int choix = 0;
    char buffer[6] = {0};
    ccpy_recur* cpy_buffer = NULL, *cpy_next=NULL;
    ccon_recur* con_buffer = NULL;
    cscr_recur* rep_buffer = NULL;
    ccam cam = {0};

    //finish preparing the indexes metadata
    index_cpy.elementsize = sizeof(i_ccpy_name);
    index_cpy.doCompare = compare_company_index_char;
    index_con.elementsize = sizeof(i_ccon_cpy);
    index_con.doCompare = compare_contact_index_int;
    index_cam.elementsize = sizeof(i_ccam_PK);
    index_cam.doCompare = compare_campaign_index_int;

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
        //display all the possibilities
        printf("\nVeuillez choisir pour quelle compagnie afficher le rapport : (1 a %ld)\n\n", list_cpy.nbelements);
        foreachList(&list_cpy, NULL, Rec_company_list);

        //ask the user to pick a one
        printf("\nChoix : ");
        fflush(stdin);
        fgets(buffer, 6, stdin);
        choix = atoi(buffer);

        //save the selected one in a buffer
        cpy_next = list_cpy.structure;
        for(int i=1 ; i<choix ; i++){
            cpy_next = *company_right(cpy_next);
        }
        cpy_buffer = (ccpy_recur*)cpy_next;
    }

    //look for all the contacts related to the chosen company and create a linked list
    searchall_index(db->fp, db->hdr.i_con_cpy, &cpy_buffer->cpy.id_cpy, &index_con, &list_con, sizeof(ccon));

    //browse through all the contacts found
    con_buffer = list_con.structure;
    for(int i=0 ; i<list_con.nbelements ; i++){
        //search for the related campaign
        searchone_index(db->fp, db->hdr.i_cam_pk, &con_buffer->con.id_cam, &index_cam, &cam, sizeof(ccam));

        //fill the report buffer with the campaign found
        rep_buffer = calloc(1, sizeof(cscr_recur));
        rep_buffer->rep.id_cam = cam.id_cam;
        rep_buffer->rep.id_job = con_buffer->con.id_job;
        rep_buffer->rep.nr_rep = con_buffer->con.nr_rep;
        strcpy(rep_buffer->rep.cam_nm, cam.nm_cam);
        strcpy(rep_buffer->rep.cam_tp, cam.tp_cam);

        //add it to a list
        insertListSorted(&list_rep, rep_buffer);

        //go to the next contact
        con_buffer = *contact_right(con_buffer);
    }

    //display all the campaigns sorted by type, according to the report requested
    printf("ID_job\tID_cam\tnr_rep\t\t\t   cam_nm\t\t      cam_type\n");
    foreachList(&list_rep, NULL, Rec_scr_report_list);

    //clean up all the lists and close the DB
    while(list_cpy.structure)
        popListTop(&list_cpy);

    while(list_con.structure)
        popListTop(&list_con);

    while(list_rep.structure)
        popListTop(&list_rep);

    fclose(db->fp);

    return 0;
}

/****************************************************************************************/
/*  I : Database from which print the screen report                                     */
/*  P : Creates and exports an aggretated report                                        */
/*          report : response rate by amount of contacts per year and camp. type        */
/*  O : -1 if not found                                                                 */
/*       0 otherwise                                                                    */
/****************************************************************************************/
int export_aggregated_report(dbc* db){

    printf("\nNot implemented yet...\n");

    return 0;
}

/****************************************************************************************/
/*  I : /                                                                               */
/*  P : Generates and exports the appointed det. report                                 */
/*          For 1 group, group companies by zone, sorted by name                        */
/*  O : -1 if error                                                                     */
/*       0 otherwise                                                                    */
/****************************************************************************************/
int export_detailed_report(dbc* db, char* nm_grp){
    //metadata for all the lists created
    t_algo_meta list_grp_nm = {NULL, 0, sizeof(cgrp_recur), compare_group_nm, swap_group, assign_group, NULL, NULL, NULL, group_right, group_left};
    t_algo_meta list_cpy_grp = {NULL, 0, sizeof(ccpy_recur), compare_company_grp, swap_company, assign_company, NULL, NULL, NULL, company_right, company_left};
//    t_algo_meta list_rep = {NULL, 0, sizeof(cscr_recur), compare_scr_report_type, swap_scr_report, assign_scr_report, NULL, NULL, NULL, scr_report_right, scr_report_left};
    //metadata for all the indexes used
    t_algo_meta index_grp = {0};
    t_algo_meta index_cpy = {0};

    int choix = 0;
    char buffer[6] = {0};
    cgrp_recur* grp_buffer = NULL, *grp_next=NULL;

    //finish preparing the indexes metadata
    index_grp.elementsize = sizeof(i_cgrp_nm);
    index_grp.doCompare = compare_group_nm_index;
    index_cpy.elementsize = sizeof(i_ccpy_grp);
    index_cpy.doCompare = compare_company_grp;

    // open the DB
    db->fp = fopen(DB_file, "rb");
    if(!db->fp){
        fprintf(stderr, "\n\nErreur a l'ouverture de la DB\n\n");
        return -1;
    }

    //search for all occurrences of the company name (creates a linked list)
    searchall_index(db->fp, db->hdr.i_grp_nm, nm_grp, &index_grp, &list_grp_nm, sizeof(cgrp));
    grp_buffer=list_grp_nm.structure;

    //if not found, error
    if(!list_grp_nm.nbelements){
        fprintf(stderr, "\n\n%s : non-trouve\n\n", nm_grp);
        return -1;
    }

    //if more than one occurrence, make the user choose which one
    if(list_grp_nm.nbelements > 1){
        //display all the possibilities
        printf("\nVeuillez choisir pour quel groupe afficher le rapport : (1 a %ld)\n\n", list_grp_nm.nbelements);
        foreachList(&list_grp_nm, NULL, Rec_group_list);

        //ask the user to pick a one
        printf("\nChoix : ");
        fflush(stdin);
        fgets(buffer, 6, stdin);
        choix = atoi(buffer);

        //save the selected one in a buffer
        grp_next = list_grp_nm.structure;
        for(int i=1 ; i<choix ; i++){
            grp_next = *group_right(grp_next);
        }
        grp_buffer = (cgrp_recur*)grp_next;
    }

    //look for all the contacts related to the chosen company and create a linked list
    searchall_index(db->fp, db->hdr.i_cpy_grp, &grp_buffer->grp.id_grp, &index_cpy, &list_cpy_grp, sizeof(ccpy));

    return 0;
}

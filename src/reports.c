#include "reports.h"

/****************************************************************************************/
/*  I : First Screen Report to compare                                                  */
/*      Second  Screen Report to compare                                                */
/*  P : Compares two reports by their campaign types, then by job ID                    */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_scr_report_type(void* a, void* b){
    cscr *tmp_a = (cscr*)a;
    cscr *tmp_b = (cscr*)b;
    int cam_comp = 0;


    cam_comp = strcmp(tmp_a->cam_tp, tmp_b->cam_tp);
    if(!cam_comp){
        if(tmp_a->id_job > tmp_b->id_job)
            return 1;
        else if (tmp_a->id_job < tmp_b->id_job)
            return -1;
        else
            return 0;
    }
    else
        return cam_comp;
}

/****************************************************************************************
* Imprime un record Screen Report depuis le buffer
****************************************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Rec_scr_report(void *rec, void *nullable){
    cscr *tmp = (cscr*)rec;

    printf("%6d %6d %6d %28s %28s\n",
           tmp->id_job,
           tmp->id_cam,
           tmp->nr_rep,
           tmp->cam_nm,
           tmp->cam_tp);

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
    meta_t list_cpy = {NULL, NULL, 0, sizeof(ccpy), compare_company_name, print_error};
    meta_t list_con = {NULL, NULL, 0, sizeof(ccon), compare_contact_cpy, print_error};
    meta_t list_rep = {NULL, NULL, 0, sizeof(cscr), compare_scr_report_type, print_error};
    //metadata for all the indexes used
    meta_t index_cpy = {0};
    meta_t index_con = {0};
    meta_t index_cam = {0};

    int choix = 0;
    char buffer[6] = {0};
    dyndata_t* next = NULL;
    ccpy* cpy_buffer = NULL;
    cscr* rep_buffer = NULL;
    ccam cam = {0};

    //finish preparing the indexes metadata
    index_cpy.elementsize = sizeof(i_ccpy_name);
    index_cpy.doCompare = compare_company_index_char;
    index_cpy.doPError = print_error;
    index_con.elementsize = sizeof(i_ccon_cpy);
    index_con.doCompare = compare_contact_index_int;
    index_con.doPError = print_error;
    index_cam.elementsize = sizeof(i_ccam_PK);
    index_cam.doCompare = compare_campaign_index_int;
    index_cam.doPError = print_error;

    // open the DB
    db->fp = fopen(DB_file, "rb");
    if(!db->fp){
        fprintf(stderr, "\n\nErreur a l'ouverture de la DB\n\n");
        return -1;
    }


    //
    //  Retrieve all the companies matching the name typed by the user
    //


    //search for all occurrences of the company name in the companies name index
    //      and create a companies list
    searchall_index(db->fp, db->hdr.i_cpy_name, nm_cpy, &index_cpy, &list_cpy);

    //if not found, error
    if(!list_cpy.nbelements){
        fprintf(stderr, "\n\n%s : non-trouve\n\n", nm_cpy);
        return -1;
    }

    //set the buffer to the first occurence found by default
    cpy_buffer=get_listelem(&list_cpy, 0);

    //if more than one occurrence, make the user choose which one
    if(list_cpy.nbelements > 1){
        //display all the possibilities
        printf("\nVeuillez choisir pour quelle compagnie afficher le rapport : (1 a %lu)\n\n", (unsigned long int)list_cpy.nbelements);
        foreachList(&list_cpy, NULL, Rec_company);

        //ask the user to pick a one
        printf("\nChoix : ");
        fflush(stdin);
        fgets(buffer, 6, stdin);
        choix = atoi(buffer);

        //save the selected one in a buffer
        cpy_buffer = get_listelem(&list_cpy, choix - 1);
    }

    //
    // Retrieve all the contacts associated to the company chosen
    //     (sorted by campaign type)
    //


    //look for all the contacts related to the chosen company and create a linked list
    searchall_index(db->fp, db->hdr.i_con_cpy, &cpy_buffer->id_cpy, &index_con, &list_con);

    //browse through all the contacts found
    next = list_con.structure;
    for(uint64_t i=0 ; i<list_con.nbelements ; i++){
        //search for the related campaign
        searchone_index(db->fp, db->hdr.i_cam_pk, &((ccon*)next->data)->id_cam, &index_cam, &cam, sizeof(ccam));

        //fill the report buffer with the campaign found
        rep_buffer = calloc(1, sizeof(cscr));
        rep_buffer->id_cam = cam.id_cam;
        rep_buffer->id_job = ((ccon*)next->data)->id_job;
        rep_buffer->nr_rep = ((ccon*)next->data)->nr_rep;
        strcpy(rep_buffer->cam_nm, cam.nm_cam);
        strcpy(rep_buffer->cam_tp, cam.tp_cam);

        //add it to a list
        insertListSorted(&list_rep, rep_buffer);

        //go to the next contact
        next = getright(next);
    }


    //
    // Display the list of final occurrences mathing the research
    //


    //display all the campaigns sorted by type, according to the report requested
    printf("ID_job\tID_cam\tnr_rep\t\t\t   cam_nm\t\t      cam_type\n");
    foreachList(&list_rep, NULL, Rec_scr_report);


    //
    // Ressources cleanup
    //


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
    meta_t list_grp_nm = {NULL, NULL, 0, sizeof(cgrp), compare_group_nm, print_error};
    meta_t list_cpy_grp = {NULL, NULL, 0, sizeof(ccpy), compare_company_grp, print_error};
    //metadata for all the indexes used
    meta_t index_grp = {0};
    meta_t index_cpy = {0};

    int choix = 0;
    char buffer[6] = {0};
    dyndata_t* group = NULL;

    //finish preparing the indexes metadata
    index_grp.elementsize = sizeof(i_cgrp_nm);
    index_grp.doCompare = compare_group_nm_index_char;
    index_grp.doPError = print_error;
    index_cpy.elementsize = sizeof(i_ccpy_grp);
    index_cpy.doCompare = compare_company_index_int;
    index_cpy.doPError = print_error;

    // open the DB
    db->fp = fopen(DB_file, "rb");
    if(!db->fp){
        fprintf(stderr, "\n\nErreur a l'ouverture de la DB\n\n");
        return -1;
    }


    //
    // Retrieve all the groups matching the name entered by the user
    //


    //search for all occurrences of the group name (creates a linked list)
    searchall_index(db->fp, db->hdr.i_grp_nm, nm_grp, &index_grp, &list_grp_nm);

    //if not found, error
    if(!list_grp_nm.nbelements){
        fprintf(stderr, "\n\n%s : non-trouve\n\n", nm_grp);
        return -1;
    }

    //select the first occurrence by default
    group=list_grp_nm.structure;

    //if more than one occurrence, make the user choose which one
    if(list_grp_nm.nbelements > 1){
        //display all the possibilities
        printf("\nVeuillez choisir pour quel groupe afficher le rapport : (1 a %lu)\n\n", (unsigned long int)list_grp_nm.nbelements);
        foreachList(&list_grp_nm, NULL, Rec_Group);

        //ask the user to pick a one
        printf("\nChoix : ");
        fflush(stdin);
        fgets(buffer, 6, stdin);
        choix = atoi(buffer);

        //save the selected one in a buffer
        group = get_listelem(&list_grp_nm, choix - 1);
    }


    //
    // Retrieve all the companies with a Group ID mathing the one chosen
    //


    //look for all the companies related to the chosen group and create a linked list
    searchall_index(db->fp, db->hdr.i_cpy_grp, &((cgrp*)group->data)->id_grp, &index_cpy, &list_cpy_grp);

    //if not found, error
    if(!list_cpy_grp.nbelements){
        fprintf(stderr, "\n\nNo company matching the group ID found\n\n");
        return -1;
    }

    fclose(db->fp);

    printf("\nList retrieved, but report creation not implemented...\n");

    return 0;
}

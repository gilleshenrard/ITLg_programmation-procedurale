/****************************************************************************************
* Dossier 2 : Database "Analyse de donnees clients"
*
* Test des routines de demo : fichiers non geres globalement
*
* Programmation procedurale 2019 - E.Bosly - Version 0
****************************************************************************************/
#include "lib/DB_Main.h"
#include "lib/DB_Country.h"
#include "lib/algo.h"
#include "lib/DB_File.h"
#include "lib/DB_Group.h"
#include "lib/DB_Job.h"
#include "lib/DB_Industry.h"
#include "lib/DB_campaign.h"
#include "lib/DB_Contact.h"
#include "lib/DB_Company.h"

void init_db(dbc*);
void tst_export_country(dbc*);
void tst_Load_country(dbc*);
void tst_Print_country(dbc*);
void tst_List_country(dbc*);
void tst_AVL_country(dbc*);
void tst_AVL_search_country(dbc*);
void tst_index_country_name(dbc* db);
void tst_search_index_country_name(dbc* db);
void tst_index_group(dbc* db);
void tst_index_job(dbc* db);
void tst_index_industry(dbc* db);
void tst_index_campaign(dbc* db);
void tst_index_contact(dbc* db);

/****************************************************************************************
* Programme principal
****************************************************************************************/
int main(void)
{
    dbc db = {0};
    init_db(&db);

//    tst_export_country(&db);
//    tst_Load_country(&db);
//    tst_Print_country(&db);
//    tst_List_country(&db);
//    tst_AVL_country(&db);
//    tst_AVL_search_country(&db);
//    tst_index_country_name(&db);
//    tst_search_index_country_name(&db);
//    tst_index_group(&db);
//    tst_index_job(&db);
//    tst_index_industry(&db);
//    tst_index_campaign(&db);
//    tst_index_contact(&db);

    if(db.cty)
        free(db.cty);

    if(db.grp)
        free(db.grp);

    if(db.job)
        free(db.job);

    if(db.ind)
        free(db.ind);

    if(db.cam)
        free(db.cam);

    if(db.con)
        free(db.con);

	return 0;
}

/****************************************************************************************/
/*  I : Database to create and populate                                                 */
/*  P : Creates the database files and populates it (export) with the files             */
/*          located in import/                                                          */
/*  O : /                                                                               */
/****************************************************************************************/
void init_db(dbc* db){
    Create_DB(db, "DB_Comp");
//    Import_CSV_Country(db);
//    Import_CSV_Group(db);
//    Import_CSV_job(db);
//    Import_CSV_industry(db);
//    Import_CSV_campaign(db);
//    Import_CSV_contact(db);

    printf("header size : %d\n", sizeof(hder));
    printf("ccty size : %d\n", sizeof(ccty));
    printf("cjob size : %d\n", sizeof(cjob));
    printf("cind size : %d\n", sizeof(cind));
    printf("cgrp size : %d\n", sizeof(cgrp));
    printf("ccam size : %d\n", sizeof(ccam));
    printf("ccon size : %d\n", sizeof(ccon));
    printf("ccpy size : %d\n", sizeof(ccpy));
    printf("i_ccty_name size : %d\n", sizeof(i_ccty_name));
    printf("i_cjob_FK size : %d\n", sizeof(i_cjob_name));
    printf("i_cind_FK size : %d\n", sizeof(i_cind_PK));
    printf("i_cgrp_FK size : %d\n", sizeof(i_cgrp_FK));
    printf("i_ccam_FK size : %d\n", sizeof(i_ccam_PK));
    printf("i_ccon_FK size : %d\n", sizeof(i_ccon_cpy));
    printf("i_ccpy_name size : %d\n", sizeof(i_ccpy_name));
}

/****************************************************************************************/
/*  I : Country database to export to CSV                                               */
/*  P : Exports all the data from the DB to a CSV file                                  */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_export_country(dbc* db){
    printf("\n--------------- tst_export_country ------------------------------------\n");
    Export_CSV_Country(db);
}

/****************************************************************************************/
/*  I : Country DB to load in memory                                                    */
/*  P : Reads a DB from a file and loads it into memory                                 */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_Load_country(dbc* db){
    printf("\n--------------- tst_Load_country --------------------------------------\n");

    Load_Country(db);
}

/****************************************************************************************/
/*  I : Country Database to print                                                       */
/*  P : Displays all records of a DB in memory                                          */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_Print_country(dbc* db){
    printf("\n--------------- tst_print_country -------------------------------------\n");
    Print_Country(db);
}

/****************************************************************************************/
/*  I : Country Database to print                                                       */
/*  P : Displays all records of a DB in memory                                          */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_List_country(dbc* db){
    t_algo_meta cty_list = {NULL, 0, sizeof(ccty_recur), compare_country_name, swap_country, assign_country, NULL, NULL, NULL, country_right, country_left};
    t_algo_meta cty_array = {db->cty+1, db->nr_cty, sizeof(ccty), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

    printf("\n--------------- tst_list_country ---------------------------------------\n");
    arrayToList(&cty_array, &cty_list, COPY);

    printf("%d countries\n", cty_list.nbelements);

    foreachList(&cty_list, NULL, Rec_Country_list);
    while(cty_list.structure)
        popListTop(&cty_list);
}

/****************************************************************************************/
/*  I : Country Database to print                                                       */
/*  P : Displays all records of a DB in memory                                          */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_AVL_country(dbc* db){
    t_algo_meta cty_avl = {NULL, 0, sizeof(ccty_recur), compare_country_name, swap_country, assign_country, NULL, get_country_height, set_country_height, country_right, country_left};
    t_algo_meta cty_array = {db->cty+1, db->nr_cty, sizeof(ccty), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    ccty_recur tmp = {*(db->cty+3), 0, NULL, NULL};

    printf("\n--------------- tst_AVL_insert ---------------------------------------\n");

    //prepare the AVL tree from the countries array
    arrayToAVL(&cty_array, &cty_avl, COPY);
    display_AVL_tree(&cty_avl, cty_avl.structure, 'T', &toString_Country);
    printf("\n\n");

    //try to add an already existing node
    printf("Trying to add an already existing country (forbidden)\n");
    printf("%d countries before\n", cty_avl.nbelements);
    insertAVL(&cty_avl, cty_avl.structure, &tmp);
    printf("%d countries after\n\n", cty_avl.nbelements);

    delete_AVL(&cty_avl, cty_avl.structure, &tmp);

    //display the countries as a list
    foreachAVL(&cty_avl, cty_avl.structure, NULL, Rec_Country_list);

    //free memory
    while(cty_avl.structure)
        delete_AVL_root(&cty_avl);
}

/****************************************************************************************/
/*  I : Country Database in which look for a key                                        */
/*  P : Tests the AVL key search method                                                 */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_AVL_search_country(dbc* db){
    t_algo_meta cty_avl = {NULL, 0, sizeof(ccty_recur), compare_country_name, swap_country, assign_country, NULL, get_country_height, set_country_height, country_right, country_left};
    t_algo_meta cty_array = {db->cty+1, db->nr_cty, sizeof(ccty), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char countries[5][16] = {"Denmark", "Portugal", "Burkina Faso", "Estonia", "Singapore"};

    printf("\n--------------- tst_AVL_search_country ---------------------------------\n");
    arrayToAVL(&cty_array, &cty_avl, COPY);

    cty_avl.doCompare = compare_country_name_char;
    for(int i=0 ; i<5 ; i++){
        printf("Searching for %12s: ", countries[i]);
        if (search_AVL(&cty_avl, cty_avl.structure, countries[i]))
            printf("found\n");
        else
            printf("not found\n");
    }

    cty_avl.doCompare = compare_country_name;
    while(cty_avl.structure)
        delete_AVL_root(&cty_avl);
}

/****************************************************************************************/
/*  I : Country Database in which create a country name index                           */
/*  P : Tests the country name index creation at the end of the database                */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_index_country_name(dbc* db){
    t_algo_meta meta = {NULL, db->nr_cty, sizeof(i_ccty_name), compare_country_index_name, swap_country_index, assign_country_index_name, assign_country_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block={&db->hdr.off_i_cty_name, &db->hdr.i_cty_name, sizeof(i_ccty_name)};
    t_datablock table_block={&db->hdr.off_cty, 0, sizeof(ccty)};
    FILE* fp = NULL;
    i_ccty_name buffer = {0};
    long offset = 0;

    printf("\n--------------- tst_index_country_name ---------------------------------\n");

    create_index_file(db, &meta, db->nr_cty, &index_block, &table_block);

    fp = fopen(DB_file, "rb");
    if(fp){
        printf("\n\t\t\tNAME\tSLOT\tLEFT\tOFFSET\tRIGHT\n\n");
        fseek(fp, db->hdr.off_i_cty_name, SEEK_SET);
        for(int i=0 ; i<db->nr_cty ; i++){
            offset = ftell(fp);
            fread(&buffer, sizeof(i_ccty_name), 1, fp);
            printf("%28s\t%6lx\t%6lx\t%6lx\t%6lx  %d\n", buffer.nm_cty, buffer.slot, buffer.s_left, offset, buffer.s_right, i+1);
        }

        fseek(fp, db->hdr.i_cty_name, SEEK_SET);
        fread(&buffer, sizeof(i_ccty_name), 1, fp);

        printf("\nNumber of elements : %ld\n", db->nr_cty);
        printf("index block offset : %lX\n", db->hdr.off_i_cty_name);
        printf("index tree root : %lX\n", db->hdr.i_cty_name);
        printf("root city name : %s\n", buffer.nm_cty);

        fclose(fp);
    }
}

/****************************************************************************************/
/*  I : Country Database in which create a country name index                           */
/*  P : Tests the country name index creation at the end of the database                */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_search_index_country_name(dbc* db){
    t_algo_meta index = {0};
    t_algo_meta list = {NULL, 0, sizeof(ccty_recur), compare_country_name, swap_country, assign_country, NULL, NULL, NULL, country_left, country_right};

    printf("\n--------------- tst_search_index_country_name --------------------------\n");

    index.elementsize = sizeof(i_ccty_name);
    index.doCompare = compare_country_index_char;

    db->fp = fopen(DB_file, "rb");
    if(db->fp){
        searchall_index(db->fp, db->hdr.i_cty_name, "France", &index, &list, sizeof(ccty));
        fclose(db->fp);
    }

    foreachList(&list, NULL, Rec_Country_list);
    while(list.structure)
        popListTop(&list);
}

/****************************************************************************************/
/*  I : Database in which create a group FK index                                       */
/*  P : Tests the group FK index creation at the end of the database                    */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_index_group(dbc* db){
    t_algo_meta grp_list = {NULL, 0, sizeof(cgrp_recur), compare_group_FK, swap_group, assign_group, NULL, NULL, NULL, group_right, group_left};
    t_algo_meta grp_array = {NULL, db->nr_grp, sizeof(cgrp), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    t_algo_meta index = {NULL, db->nr_grp, sizeof(i_cgrp_FK), compare_group_FK_index, swap_group_index, assign_group_index_FK, assign_group_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block={&db->hdr.off_i_grp_fk, &db->hdr.i_grp_fk, sizeof(i_cgrp_FK)};
    t_datablock table_block={&db->hdr.off_grp, 0, sizeof(cgrp)};
    FILE* fp = NULL;
    i_cgrp_FK buffer = {0};
    long offset = 0;

    printf("\n--------------------- tst_index_group -------------------------------\n");
    Export_CSV_Group(db);
    Load_Group(db);
    Print_Group(db);

    // test list creation
    printf("\n\n========= List of groups sorted by their FK : ==============\n\n");
    grp_array.structure = db->grp;
    arrayToList(&grp_array, &grp_list, COPY);
    printf("%d groups\n", grp_list.nbelements);
    foreachList(&grp_list, NULL, Rec_group_list);
    while(grp_list.structure)
        popListTop(&grp_list);

    //test index creation
    printf("\n\n======== Index of groups sorted by their FK : =============\n\n");
    create_index_file(db, &index, db->nr_grp, &index_block, &table_block);

    fp = fopen(DB_file, "rb");
    if(fp){
        printf("\nFK\t  SLOT\t  LEFT\t OFFSET\t RIGHT\n\n");
        fseek(fp, db->hdr.off_i_grp_fk, SEEK_SET);
        for(int i=0 ; i<db->nr_grp ; i++){
            offset = ftell(fp);
            fread(&buffer, sizeof(i_cgrp_FK), 1, fp);
            printf("%d\t%6lx\t%6lx\t%6lx\t%6lx  %d\n", buffer.cty_id, buffer.slot, buffer.s_left, offset, buffer.s_right, i+1);
        }

        fseek(fp, db->hdr.i_grp_fk, SEEK_SET);
        fread(&buffer, sizeof(i_cgrp_FK), 1, fp);

        printf("\nNumber of elements : %ld\n", db->nr_grp);
        printf("index block offset : %lX\n", db->hdr.off_i_grp_fk);
        printf("index tree root : %lX\n", db->hdr.i_grp_fk);
        printf("root FK : %d\n", buffer.cty_id);

        fclose(fp);
    }
}

/****************************************************************************************/
/*  I : Database in which create a job name index                                       */
/*  P : Tests the job name index creation at the end of the database                    */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_index_job(dbc* db){
    t_algo_meta job_list = {NULL, 0, sizeof(cjob_recur), compare_job_name, swap_job, assign_job, NULL, NULL, NULL, job_right, job_left};
    t_algo_meta job_array = {NULL, db->nr_job, sizeof(cjob), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    t_algo_meta index = {NULL, db->nr_job, sizeof(i_cjob_name), compare_job_index_name, swap_job_index, assign_job_index_name, assign_job_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block={&db->hdr.off_i_job_name, &db->hdr.i_job_name, sizeof(i_cjob_name)};
    t_datablock table_block={&db->hdr.off_job, 0, sizeof(cjob)};
    FILE* fp = NULL;
    i_cjob_name buffer = {0};
    long offset = 0;

    printf("\n--------------------- tst_index_job -------------------------------\n");
    Export_CSV_job(db);
    Load_job(db);
    Print_job(db);

    // test list creation
    printf("\n\n========= List of jobs sorted by their name : ==============\n\n");
    job_array.structure = db->job;
    arrayToList(&job_array, &job_list, COPY);
    printf("%d jobs\n", job_list.nbelements);
    foreachList(&job_list, NULL, Rec_job_list);
    while(job_list.structure)
        popListTop(&job_list);

    //test index creation
    printf("\n\n======== Index of jobs sorted by their name : =============\n\n");
    create_index_file(db, &index, db->nr_job, &index_block, &table_block);

    fp = fopen(DB_file, "rb");
    if(fp){
        printf("\n\t\t\t    NAME\t  SLOT\t  LEFT\t OFFSET\t RIGHT\n\n");
        fseek(fp, db->hdr.off_i_job_name, SEEK_SET);
        for(int i=0 ; i<db->nr_job ; i++){
            offset = ftell(fp);
            fread(&buffer, sizeof(i_cjob_name), 1, fp);
            printf("%32s\t%6lx\t%6lx\t%6lx\t%6lx  %d\n", buffer.nm_job, buffer.slot, buffer.s_left, offset, buffer.s_right, i+1);
        }

        fseek(fp, db->hdr.i_job_name, SEEK_SET);
        fread(&buffer, sizeof(i_cjob_name), 1, fp);

        printf("\nNumber of elements : %ld\n", db->nr_job);
        printf("index block offset : %lX\n", db->hdr.off_i_job_name);
        printf("index tree root : %lX\n", db->hdr.i_job_name);
        printf("root FK : %s\n", buffer.nm_job);

        fclose(fp);
    }
}

/****************************************************************************************/
/*  I : Database in which create a industry PK index                                    */
/*  P : Tests the industry PK index creation at the end of the database                 */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_index_industry(dbc* db){
    t_algo_meta ind_list = {NULL, 0, sizeof(cind_recur), compare_industry_PK, swap_industry, assign_industry, NULL, NULL, NULL, industry_right, industry_left};
    t_algo_meta ind_array = {NULL, db->nr_ind, sizeof(cind), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    t_algo_meta index = {NULL, db->nr_ind, sizeof(i_cind_PK), compare_industry_PK_index, swap_industry_index, assign_industry_index_PK, assign_industry_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block={&db->hdr.off_i_ind_pk, &db->hdr.i_ind_pk, sizeof(i_cind_PK)};
    t_datablock table_block={&db->hdr.off_ind, 0, sizeof(cind)};
    FILE* fp = NULL;
    i_cind_PK buffer = {0};
    long offset = 0;

    printf("\n--------------------- tst_index_industry -------------------------------\n");
    Export_CSV_industry(db);
    Load_industry(db);
    Print_industry(db);

    // test list creation
    printf("\n\n========= List of industries sorted by their FK : ==============\n\n");
    ind_array.structure = db->ind;
    arrayToList(&ind_array, &ind_list, COPY);
    printf("%d industries\n", ind_list.nbelements);
    foreachList(&ind_list, NULL, Rec_industry_list);
    while(ind_list.structure)
        popListTop(&ind_list);

    //test index creation
    printf("\n\n======== Index of industries sorted by their FK : =============\n\n");
    create_index_file(db, &index, db->nr_ind, &index_block, &table_block);

    fp = fopen(DB_file, "rb");
    if(fp){
        printf("\nPK\t  SLOT\t  LEFT\t OFFSET\t RIGHT\n\n");
        fseek(fp, db->hdr.off_i_ind_pk, SEEK_SET);
        for(int i=0 ; i<db->nr_ind ; i++){
            offset = ftell(fp);
            fread(&buffer, sizeof(i_cind_PK), 1, fp);
            printf("%d\t%6lx\t%6lx\t%6lx\t%6lx  %d\n", buffer.ind_id, buffer.slot, buffer.s_left, offset, buffer.s_right, i+1);
        }

        fseek(fp, db->hdr.i_ind_pk, SEEK_SET);
        fread(&buffer, sizeof(i_cind_PK), 1, fp);

        printf("\nNumber of elements : %ld\n", db->nr_ind);
        printf("index block offset : %lX\n", db->hdr.off_i_ind_pk);
        printf("index tree root : %lX\n", db->hdr.i_ind_pk);
        printf("root FK : %d\n", buffer.ind_id);

        fclose(fp);
    }
}

/****************************************************************************************/
/*  I : Database in which create a campaign PK index                                    */
/*  P : Tests the campaign PK index creation at the end of the database                 */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_index_campaign(dbc* db){
    t_algo_meta campaign_list = {NULL, 0, sizeof(ccam_recur), compare_campaign_PK, swap_campaign, assign_campaign, NULL, NULL, NULL, campaign_right, campaign_left};
    t_algo_meta campaign_array = {NULL, db->nr_cam, sizeof(ccam), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    t_algo_meta index = {NULL, db->nr_cam, sizeof(i_ccam_PK), compare_campaign_PK_index, swap_campaign_index, assign_campaign_index_PK, assign_campaign_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block={&db->hdr.off_i_cam_pk, &db->hdr.i_cam_pk, sizeof(i_ccam_PK)};
    t_datablock table_block={&db->hdr.off_cam, 0, sizeof(ccam)};
    FILE* fp = NULL;
    i_ccam_PK buffer = {0};
    long offset = 0;

    printf("\n--------------------- tst_index_campaign -------------------------------\n");
    Export_CSV_campaign(db);
    Load_campaign(db);
    Print_campaign(db);

    // test list creation
    printf("\n\n========= List of campaigns sorted by their PK : ==============\n\n");
    campaign_array.structure = db->cam;
    arrayToList(&campaign_array, &campaign_list, COPY);
    printf("%d campaigns\n", campaign_list.nbelements);
    foreachList(&campaign_list, NULL, Rec_campaign_list);
    while(campaign_list.structure)
        popListTop(&campaign_list);

    //test index creation
    printf("\n\n======== index of campaigns sorted by their PK : =============\n\n");
    create_index_file(db, &index, db->nr_cam, &index_block, &table_block);

    fp = fopen(DB_file, "rb");
    if(fp){
        printf("\nPK\t  SLOT\t  LEFT\t OFFSET\t RIGHT\n\n");
        fseek(fp, db->hdr.off_i_cam_pk, SEEK_SET);
        for(int i=0 ; i<db->nr_cam ; i++){
            offset = ftell(fp);
            fread(&buffer, sizeof(i_ccam_PK), 1, fp);
            printf("%d\t%6lx\t%6lx\t%6lx\t%6lx  %d\n", buffer.cam_id, buffer.slot, buffer.s_left, offset, buffer.s_right, i+1);
        }

        fseek(fp, db->hdr.i_cam_pk, SEEK_SET);
        fread(&buffer, sizeof(i_ccam_PK), 1, fp);

        printf("\nNumber of elements : %ld\n", db->nr_cam);
        printf("index block offset : %lX\n", db->hdr.off_i_cam_pk);
        printf("index tree root : %lX\n", db->hdr.i_cam_pk);
        printf("root FK : %d\n", buffer.cam_id);

        fclose(fp);
    }
}

/****************************************************************************************/
/*  I : Database in which create a contact PK index                                    */
/*  P : Tests the contact PK index creation at the end of the database                 */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_index_contact(dbc* db){
    t_algo_meta contact_list = {NULL, 0, sizeof(ccon_recur), compare_contact_cpy, swap_contact, assign_contact, NULL, NULL, NULL, contact_right, contact_left};
    t_algo_meta contact_array = {NULL, db->nr_con, sizeof(ccon), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    t_algo_meta index = {NULL, db->nr_con, sizeof(i_ccon_cpy), compare_contact_cpy_index, swap_contact_index, assign_contact_index_cpy, assign_contact_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block={&db->hdr.off_i_con_cpy, &db->hdr.i_con_cpy, sizeof(i_ccon_cpy)};
    t_datablock table_block={&db->hdr.off_con, 0, sizeof(ccon)};
    FILE* fp = NULL;
    i_ccon_cpy buffer = {0};
    long offset = 0;

    printf("\n--------------------- tst_index_contact -------------------------------\n");
    Export_CSV_contact(db);
    Load_contact(db);
    Print_contact(db);

    // test list creation
    printf("\n\n========= List of contacts sorted by their PK : ==============\n\n");
    contact_array.structure = db->con;
    arrayToList(&contact_array, &contact_list, COPY);
    printf("%d contacts\n", contact_list.nbelements);
    foreachList(&contact_list, NULL, Rec_contact_list);
    while(contact_list.structure)
        popListTop(&contact_list);

    //test index creation
    printf("\n\n======== index of contacts sorted by their PK : =============\n\n");
    create_index_file(db, &index, db->nr_con, &index_block, &table_block);

    fp = fopen(DB_file, "rb");
    if(fp){
        printf("\nPK\t  SLOT\t  LEFT\t OFFSET\t RIGHT\n\n");
        fseek(fp, db->hdr.off_i_con_cpy, SEEK_SET);
        for(int i=0 ; i<db->nr_con ; i++){
            offset = ftell(fp);
            fread(&buffer, sizeof(i_ccon_cpy), 1, fp);
            printf("%d\t%6lx\t%6lx\t%6lx\t%6lx  %d\n", buffer.cam_id, buffer.slot, buffer.s_left, offset, buffer.s_right, i+1);
        }

        fseek(fp, db->hdr.i_con_cpy, SEEK_SET);
        fread(&buffer, sizeof(i_ccon_cpy), 1, fp);

        printf("\nNumber of elements : %ld\n", db->nr_con);
        printf("index block offset : %lX\n", db->hdr.off_i_con_cpy);
        printf("index tree root : %lX\n", db->hdr.i_con_cpy);
        printf("root FK : %d\n", buffer.cam_id);

        fclose(fp);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
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
#include "lib/reports.h"

#define PG_SIZE 32

char DB_name[32] = "DB_Comp";

void init_db(dbc* db);
char menu(int, char[][32]);
int menu_countries(dbc*);
int menu_companies(dbc*);
int menu_campaigns(dbc*);

int main(int argc, char *argv[])
{
    dbc db = {0};
    char choice=0;
    char princ_menu[10][32]={"Main menu",
                            "Menu des Pays",
                            "Menu des Compagnies",
                            "Menu des Campagnes",
                            "Menu des Contacts",
                            "Menu des Groupes",
                            "Menu des Industries",
                            "Menu des Jobs",
                            "Changer de DB",
                            "Quitter"};

    init_db(&db);

    do{
        //let the user make a choice in the main menu
        choice = menu(sizeof(princ_menu)/32, princ_menu);
        switch(choice){
            case '0':   //Countries menu
                menu_countries(&db);
                break;

            case '1':   //Companies menu
                menu_companies(&db);
                break;

            case '2':   //Campaigns menu
                menu_campaigns(&db);
                break;

            case '3':   //Contacts menu
                break;

            case '4':   //Groups menu
                break;

            case '5':   //Industries menu
                break;

            case '6':   //Jobs menu
                break;

            case '7':   //Change DB menu
                break;

            default:
                break;
        }
        if(choice != 27){
            printf("\nAppuyez sur une touche pour continuer ");
            fflush(stdin);
            getch();
        }
    }while(choice!=27);

    if(db.cpy)
        free(db.cpy);
    if(db.con)
        free(db.con);
    if(db.cty)
        free(db.cty);
    if(db.grp)
        free(db.grp);
    if(db.ind)
        free(db.ind);
    if(db.job)
        free(db.job);

    return EXIT_SUCCESS;
}

/****************************************************************************************/
/*  I : Database to create                                                              */
/*  P : Creates a database file and initializes it with the required tables and indexes */
/*  O : /                                                                               */
/****************************************************************************************/
void init_db(dbc* db){
    //company metadata
    t_algo_meta index_cpy = {NULL, db->nr_cpy, sizeof(i_ccpy_name), compare_company_index_name, swap_company_index, assign_company_index_name, assign_company_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block_cpy = {&db->hdr.off_i_cpy_name, &db->hdr.i_cpy_name, sizeof(i_ccpy_name)};
    t_datablock table_block_cpy = {&db->hdr.off_cpy, 0, sizeof(ccpy)};
    //contacts metadata
    t_algo_meta index_con = {NULL, db->nr_con, sizeof(i_ccon_cpy), compare_contact_cpy_index, swap_contact_index, assign_contact_index_cpy, assign_contact_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block_con = {&db->hdr.off_i_con_cpy, &db->hdr.i_con_cpy, sizeof(i_ccon_cpy)};
    t_datablock table_block_con = {&db->hdr.off_con, 0, sizeof(ccon)};
    //campaign metadata
    t_algo_meta index_cam = {NULL, db->nr_cam, sizeof(i_ccam_PK), compare_campaign_PK_index, swap_campaign_index, assign_campaign_index_PK, assign_campaign_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block_cam = {&db->hdr.off_i_cam_pk, &db->hdr.i_cam_pk, sizeof(i_ccam_PK)};
    t_datablock table_block_cam = {&db->hdr.off_cam, 0, sizeof(ccam)};
    //country metadata
    t_algo_meta index_cty = {NULL, db->nr_cty, sizeof(i_ccty_name), compare_country_index_name, swap_country_index, assign_country_index_name, assign_country_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block_cty = {&db->hdr.off_i_cty_name, &db->hdr.i_cty_name, sizeof(i_ccty_name)};
    t_datablock table_block_cty = {&db->hdr.off_cty, 0, sizeof(ccty)};
    //group metadata
    t_algo_meta index_grp = {NULL, db->nr_grp, sizeof(i_cgrp_FK), compare_group_FK_index, swap_group_index, assign_group_index_FK, assign_group_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block_grp = {&db->hdr.off_i_grp_fk, &db->hdr.i_grp_fk, sizeof(i_cgrp_FK)};
    t_datablock table_block_grp = {&db->hdr.off_grp, 0, sizeof(cgrp)};
    //industry metadata
    t_algo_meta index_ind = {NULL, db->nr_ind, sizeof(i_cind_PK), compare_industry_PK_index, swap_industry_index, assign_industry_index_PK, assign_industry_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block_ind = {&db->hdr.off_i_ind_pk, &db->hdr.i_ind_pk, sizeof(i_cind_PK)};
    t_datablock table_block_ind = {&db->hdr.off_ind, 0, sizeof(cind)};
    //job metadata
    t_algo_meta index_job = {NULL, db->nr_job, sizeof(i_cjob_name), compare_job_index_name, swap_job_index, assign_job_index_name, assign_job_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block_job = {&db->hdr.off_i_job_name, &db->hdr.i_job_name, sizeof(i_cjob_name)};
    t_datablock table_block_job = {&db->hdr.off_job, 0, sizeof(cjob)};

    Create_DB(db, DB_name);
    Import_CSV_company(db);
    Import_CSV_contact(db);
    Import_CSV_campaign(db);
    Import_CSV_Country(db);
    Load_Country(db);
    Import_CSV_Group(db);
    Import_CSV_industry(db);
    Import_CSV_job(db);

    create_index_file(db, &index_cpy, db->nr_cpy, &index_block_cpy, &table_block_cpy);
    create_index_file(db, &index_con, db->nr_con, &index_block_con, &table_block_con);
    create_index_file(db, &index_cam, db->nr_cam, &index_block_cam, &table_block_cam);
    create_index_file(db, &index_cty, db->nr_cty, &index_block_cty, &table_block_cty);
    create_index_file(db, &index_grp, db->nr_grp, &index_block_grp, &table_block_grp);
    create_index_file(db, &index_ind, db->nr_ind, &index_block_ind, &table_block_ind);
    create_index_file(db, &index_job, db->nr_job, &index_block_job, &table_block_job);
}

/************************************************************/
/*  I : Number of possible sections in the menu             */
/*      Sections of the menu to display                     */
/*  P : Displays the menu and returns the user's choice     */
/*  O : Choice                                              */
/************************************************************/
char menu(int i, char sections[i][32]){
    int j;
    char choice = 0;

    system("cls");

    printf("****************************************\n");
    printf("\t\t%s\n", sections[0]);
    printf("****************************************\n");
    printf("Current file : %s\n\n", DB_name);
    for(j=1; j<i-1; j++)
        printf("%d) %s\n", j-1, sections[j]);
    printf("ESC) %s\n", sections[j]);

    printf("\nEffectuez votre choix : ");
    fflush(stdin);
    choice = getch();
    printf("\n");

    return choice;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Handles the menu specific to Countries              */
/*  O : -1 if error                                         */
/*       0 otherwise                                        */
/************************************************************/
int menu_countries(dbc* db){
    char choice=0;
    t_algo_meta cty_list = {NULL, 0, sizeof(ccty_recur), compare_country_name, swap_country, assign_country, NULL, NULL, NULL, country_right, country_left};
    t_algo_meta cty_array = {NULL, db->nr_cty, sizeof(ccty), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char menu_cty[4][32]={  "Menu des Pays",
                            "Lister les pays",
                            "Exporter les pays",
                            "Menu principal"};

    Load_Country(db);
    cty_array.structure = db->cty;
    arrayToList(&cty_array, &cty_list, COPY);

    do{
        choice = menu(sizeof(menu_cty)/32, menu_cty);
        switch(choice){
            case '0':
                foreachList(&cty_list, NULL, Rec_Country_list);
                break;

            case '1':
                Export_CSV_Country(db);
                break;

            default:
                break;
        }
        if(choice != 27){
            printf("\nAppuyez sur une touche pour continuer ");
            fflush(stdin);
            getch();
        }
    }while(choice!=27);

    while(cty_list.structure)
        popListTop(&cty_list);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Handles the menu specific to Companies              */
/*  O : -1 if error                                         */
/*       0 otherwise                                        */
/************************************************************/
int menu_companies(dbc* db){
    char choice=0, i;
    long j = 0;
    ccpy_recur *companies = NULL;
    void **right = NULL;
    t_algo_meta cpy_list = {NULL, 0, sizeof(ccpy_recur), compare_company_name, swap_company, assign_company, NULL, NULL, NULL, company_right, company_left};
    t_algo_meta cpy_array = {NULL, db->nr_cpy, sizeof(ccpy), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char menu_cpy[4][32]={  "Menu des Pays",
                            "Lister les pays",
                            "Exporter les pays",
                            "Menu principal"};

    Load_company(db);
    cpy_array.structure = db->cpy;
    arrayToList(&cpy_array, &cpy_list, COPY);

    do{
        choice = menu(sizeof(menu_cpy)/32, menu_cpy);
        switch(choice){
            case '0': //paginated listing of all the countries
                //create a list with PG_SIZE elements from the loaded array
                companies = cpy_list.structure;
                while(j<cpy_list.nbelements && choice!='q'){
                    i = 0;
                    while(i<PG_SIZE && j < cpy_list.nbelements){
                        Rec_company_list(companies, NULL);
                        right = company_right(companies);
                        companies = *((ccpy_recur**)right);
                        i++;
                        j++;
                    }

                    //prepare to set a new list
                    i = 0;

                    //ask the user if he wants to continue
                    printf("\nQ : arreter, ESPACE : continuer\n");
                    choice = getch();
                }
                break;

            case '1': //export the countries to a CSV file
                Export_CSV_company(db);
                break;

            default:
                break;
        }
        if(choice != 27){
            printf("\nAppuyez sur une touche pour continuer ");
            fflush(stdin);
            getch();
        }
    }while(choice!=27);

    while(cpy_list.structure)
        popListTop(&cpy_list);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Handles the menu specific to Campaigns              */
/*  O : -1 if error                                         */
/*       0 otherwise                                        */
/************************************************************/
int menu_campaigns(dbc* db){
    char choice=0, i;
    long j = 0;
    ccam_recur *campaign = NULL;
    void **right = NULL;
    t_algo_meta cam_list = {NULL, 0, sizeof(ccam_recur), compare_campaign_PK, swap_campaign, assign_campaign, NULL, NULL, NULL, campaign_right, campaign_left};
    t_algo_meta cam_array = {NULL, db->nr_cam, sizeof(ccam), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char menu_cam[4][32]={  "Menu des Campagnes",
                            "Lister les campagnes",
                            "Exporter les campagnes",
                            "Menu principal"};

    Load_campaign(db);
    cam_array.structure = db->cam;
    arrayToList(&cam_array, &cam_list, COPY);

    do{
        choice = menu(sizeof(menu_cam)/32, menu_cam);
        switch(choice){
            case '0': //paginated listing of all the countries
                //create a list with PG_SIZE elements from the loaded array
                campaign = cam_list.structure;
                while(j<cam_list.nbelements && choice!='q'){
                    i = 0;
                    while(i<PG_SIZE && j < cam_list.nbelements){
                        Rec_campaign_list(campaign, NULL);
                        right = campaign_right(campaign);
                        campaign = *((ccam_recur**)right);
                        i++;
                        j++;
                    }

                    //prepare to set a new list
                    i = 0;

                    //ask the user if he wants to continue
                    printf("\nQ : arreter, ESPACE : continuer\n");
                    choice = getch();
                }
                break;

            case '1': //export the countries to a CSV file
                Export_CSV_campaign(db);
                break;

            default:
                break;
        }
        if(choice != 27){
            printf("\nAppuyez sur une touche pour continuer ");
            fflush(stdin);
            getch();
        }
    }while(choice!=27);

    while(cam_list.structure)
        popListTop(&cam_list);

    return 0;
}

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

void create_db(dbc* db);
void load_db(dbc* db);
char menu(int, char[][32]);
int menu_countries(dbc*);
int menu_companies(dbc*);
int menu_campaigns(dbc*);
int menu_contacts(dbc*);
int menu_groups(dbc*);
int menu_industries(dbc*);
int menu_jobs(dbc*);
int gen_screen_report(dbc*);

int main(int argc, char *argv[])
{
    dbc db = {0};
    char choice=0;
    char princ_menu[14][32]={"Main menu",
                            "Menu des Pays",
                            "Menu des Compagnies",
                            "Menu des Campagnes",
                            "Menu des Contacts",
                            "Menu des Groupes",
                            "Menu des Industries",
                            "Menu des Jobs",
                            "Creer la DB",
                            "Charger la DB en memoire",
                            "Generer le rapport ecran",
                            "Generer le rapport agrege",
                            "Generer le rapport detaille",
                            "Quitter"};

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
                menu_contacts(&db);
                break;

            case '4':   //Groups menu
                menu_groups(&db);
                break;

            case '5':   //Industries menu
                menu_industries(&db);
                break;

            case '6':   //Jobs menu
                menu_jobs(&db);
                break;

            case '7':   //Create DB
                create_db(&db);
                break;

            case '8':   //Load DB in memory
                load_db(&db);
                break;

            case '9':   //Print screen report
                gen_screen_report(&db);
                break;

            case 'a':   //Export aggregated report
                export_aggregated_report(&db);
                break;

            case 'b':   //Export detailed report
                export_detailed_report(&db);
                break;

            default:
                break;
        }
        if(choice != 27)
            system("pause");

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
void create_db(dbc* db){
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
    Import_CSV_Group(db);
    Import_CSV_industry(db);
    Import_CSV_job(db);

    printf("Creating the index for Companies (by name) : ");
    create_index_file(db, &index_cpy, db->nr_cpy, &index_block_cpy, &table_block_cpy);
    printf("done\n");
    printf("Creating the index for Contacts (by company FK) : ");
    create_index_file(db, &index_con, db->nr_con, &index_block_con, &table_block_con);
    printf("done\n");
    printf("Creating the index for Campaigns (by ID) : ");
    create_index_file(db, &index_cam, db->nr_cam, &index_block_cam, &table_block_cam);
    printf("done\n");
    printf("Creating the index for Countries (by name) : ");
    create_index_file(db, &index_cty, db->nr_cty, &index_block_cty, &table_block_cty);
    printf("done\n");
    printf("Creating the index for Groups (by FK) : ");
    create_index_file(db, &index_grp, db->nr_grp, &index_block_grp, &table_block_grp);
    printf("done\n");
    printf("Creating the index for Industries (by ID) : ");
    create_index_file(db, &index_ind, db->nr_ind, &index_block_ind, &table_block_ind);
    printf("done\n");
    printf("Creating the index for Jobs (by name) : ");
    create_index_file(db, &index_job, db->nr_job, &index_block_job, &table_block_job);
    printf("done\n");
}

/****************************************************************************************/
/*  I : Database to load in memory                                                      */
/*  P : Reads the whole database and creates arrays containing the data                 */
/*  O : /                                                                               */
/****************************************************************************************/
void load_db(dbc* db){
    system("cls");
    printf("Loading countries : ");
    Load_Country(db);
    printf("done\n");
    printf("Loading companies : ");
    Load_company(db);
    printf("done\n");
    printf("Loading campaigns : ");
    Load_campaign(db);
    printf("done\n");
    printf("Loading contacts : ");
    Load_contact(db);
    printf("done\n");
    printf("Loading groups : ");
    Load_Group(db);
    printf("done\n");
    printf("Loading industries: ");
    Load_industry(db);
    printf("done\n");
    printf("Loading jobs : ");
    Load_job(db);
    printf("done\n\n");
    system("pause");
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
        if(j <= 10)
            printf("%d) %s\n", j-1, sections[j]);
        else
            printf("%c) %s\n", j+86, sections[j]);
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
        if(choice != 27)
            system("pause");

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
        if(choice != 27)
            system("pause");

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
        if(choice != 27)
            system("pause");

    }while(choice!=27);

    while(cam_list.structure)
        popListTop(&cam_list);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Handles the menu specific to Contacts               */
/*  O : -1 if error                                         */
/*       0 otherwise                                        */
/************************************************************/
int menu_contacts(dbc* db){
    char choice=0, i;
    long j = 0;
    ccon_recur *contact = NULL;
    void **right = NULL;
    t_algo_meta con_list = {NULL, 0, sizeof(ccon_recur), compare_contact_cpy, swap_contact, assign_contact, NULL, NULL, NULL, contact_right, contact_left};
    t_algo_meta con_array = {NULL, db->nr_con, sizeof(ccon), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char menu_con[4][32]={  "Menu des Contacts",
                            "Lister les contacts",
                            "Exporter les contacts",
                            "Menu principal"};

    con_array.structure = db->con;
    arrayToList(&con_array, &con_list, COPY);

    do{
        choice = menu(sizeof(menu_con)/32, menu_con);
        switch(choice){
            case '0': //paginated listing of all the countries
                //create a list with PG_SIZE elements from the loaded array
                contact = con_list.structure;
                while(j<con_list.nbelements && choice!='q'){
                    i = 0;
                    while(i<PG_SIZE && j < con_list.nbelements){
                        Rec_contact_list(contact, NULL);
                        right = contact_right(contact);
                        contact = *((ccon_recur**)right);
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
                Export_CSV_contact(db);
                break;

            default:
                break;
        }
        if(choice != 27)
            system("pause");

    }while(choice!=27);

    while(con_list.structure)
        popListTop(&con_list);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Handles the menu specific to Groupes                */
/*  O : -1 if error                                         */
/*       0 otherwise                                        */
/************************************************************/
int menu_groups(dbc* db){
    char choice=0, i;
    long j = 0;
    cgrp_recur *group = NULL;
    void **right = NULL;
    t_algo_meta grp_list = {NULL, 0, sizeof(cgrp_recur), compare_group_FK, swap_group, assign_group, NULL, NULL, NULL, group_right, group_left};
    t_algo_meta grp_array = {NULL, db->nr_grp, sizeof(cgrp), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char menu_grp[4][32]={  "Menu des groupes",
                            "Lister les groupes",
                            "Exporter les groupes",
                            "Menu principal"};

    grp_array.structure = db->grp;
    arrayToList(&grp_array, &grp_list, COPY);

    do{
        choice = menu(sizeof(menu_grp)/32, menu_grp);
        switch(choice){
            case '0': //paginated listing of all the countries
                //create a list with PG_SIZE elements from the loaded array
                group = grp_list.structure;
                while(j<grp_list.nbelements && choice!='q'){
                    i = 0;
                    while(i<PG_SIZE && j < grp_list.nbelements){
                        Rec_group_list(group, NULL);
                        right = group_right(group);
                        group = *((cgrp_recur**)right);
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
                Export_CSV_Group(db);
                break;

            default:
                break;
        }
        if(choice != 27)
            system("pause");

    }while(choice!=27);

    while(grp_list.structure)
        popListTop(&grp_list);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Handles the menu specific to Industries             */
/*  O : -1 if error                                         */
/*       0 otherwise                                        */
/************************************************************/
int menu_industries(dbc* db){
    char choice=0, i;
    long j = 0;
    cind_recur *industry = NULL;
    void **right = NULL;
    t_algo_meta ind_list = {NULL, 0, sizeof(cind_recur), compare_industry_PK, swap_industry, assign_industry, NULL, NULL, NULL, industry_right, industry_left};
    t_algo_meta ind_array = {NULL, db->nr_ind, sizeof(cind), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char menu_ind[4][32]={  "Menu des Industries",
                            "Lister les industries",
                            "Exporter les industries",
                            "Menu principal"};

    ind_array.structure = db->ind;
    arrayToList(&ind_array, &ind_list, COPY);

    do{
        choice = menu(sizeof(menu_ind)/32, menu_ind);
        switch(choice){
            case '0': //paginated listing of all the countries
                //create a list with PG_SIZE elements from the loaded array
                industry = ind_list.structure;
                while(j<ind_list.nbelements && choice!='q'){
                    i = 0;
                    while(i<PG_SIZE && j < ind_list.nbelements){
                        Rec_industry_list(industry, NULL);
                        right = industry_right(industry);
                        industry = *((cind_recur**)right);
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
                Export_CSV_industry(db);
                break;

            default:
                break;
        }
        if(choice != 27)
            system("pause");

    }while(choice!=27);

    while(ind_list.structure)
        popListTop(&ind_list);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Handles the menu specific to Jobs                   */
/*  O : -1 if error                                         */
/*       0 otherwise                                        */
/************************************************************/
int menu_jobs(dbc* db){
    char choice=0, i;
    long j = 0;
    cjob_recur *job = NULL;
    void **right = NULL;
    t_algo_meta job_list = {NULL, 0, sizeof(cjob_recur), compare_job_name, swap_job, assign_job, NULL, NULL, NULL, job_right, job_left};
    t_algo_meta job_array = {NULL, db->nr_job, sizeof(cjob), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char menu_job[4][32]={  "Menu des Jobs",
                            "Lister les jobs",
                            "Exporter les jobs",
                            "Menu principal"};

    job_array.structure = db->job;
    arrayToList(&job_array, &job_list, COPY);

    do{
        choice = menu(sizeof(menu_job)/32, menu_job);
        switch(choice){
            case '0': //paginated listing of all the countries
                //create a list with PG_SIZE elements from the loaded array
                job = job_list.structure;
                while(j<job_list.nbelements && choice!='q'){
                    i = 0;
                    while(i<PG_SIZE && j < job_list.nbelements){
                        Rec_job_list(job, NULL);
                        right = job_right(job);
                        job = *((cjob_recur**)right);
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
                Export_CSV_job(db);
                break;

            default:
                break;
        }
        if(choice != 27)
            system("pause");

    }while(choice!=27);

    while(job_list.structure)
        popListTop(&job_list);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Generates and prints the appointed screen report    */
/*          Contacts for 1 company, ordered by campaign type*/
/*  O : -1 if error                                         */
/*       0 otherwise                                        */
/************************************************************/
int gen_screen_report(dbc* db){
    char com[64] = "0";

    system("cls");
    printf("****************************************\n");
    printf("*           Rapport ecran :            *\n");
    printf("*  Contacts pour une compagnie, tries  *\n");
    printf("*         par type de campagne         *\n");
    printf("****************************************\n");

    printf("\n\nVeuillez taper le nom d'une compagnie : ");
    fgets(com, sizeof(com), stdin);
    if ((strlen(com) > 0) && (com[strlen(com)-1] == '\n'))
        com[strlen(com)-1] = '\0';

    printf("\n\nGeneration du rapport\n");
    print_screen_report(db, com);

    return 0;
}

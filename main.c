#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include "reports.h"
#include "screen.h"
#include "DB_Campaign.h"

#define PG_SIZE 32

char DB_name[32] = "DB_Comp";

void create_db(dbc* db);
int load_db(dbc* db);
char menu(int, char[][32]);
int menu_countries(dbc*);
int menu_companies(dbc*);
int menu_campaigns(dbc*);
int menu_contacts(dbc*);
int menu_groups(dbc*);
int menu_industries(dbc*);
int menu_jobs(dbc*);
int gen_screen_report(dbc*);
int gen_detailed_report(dbc*);

#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
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
                gen_detailed_report(&db);
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
    //company name metadata
    meta_t index_cpy_nm = {NULL, NULL, db->hdr.nr_cpy, sizeof(i_ccpy_name), compare_company_index_name, print_error};
    t_datablock index_block_cpy_nm = {&db->hdr.off_i_cpy_name, &db->hdr.i_cpy_name, sizeof(i_ccpy_name), assign_company_index_nm_slot, assign_company_index_name};
    t_datablock table_block_cpy = {&db->hdr.off_cpy, 0, sizeof(ccpy), NULL, NULL};
    //company group ID metadata
    meta_t index_cpy_grp = {NULL, NULL, db->hdr.nr_cpy, sizeof(i_ccpy_grp), compare_company_index_grp, print_error};
    t_datablock index_block_cpy_grp = {&db->hdr.off_i_cpy_grp, &db->hdr.i_cpy_grp, sizeof(i_ccpy_grp), assign_company_index_grp_slot, assign_company_index_grp};
    //contacts metadata
    meta_t index_con = {NULL, NULL, db->hdr.nr_con, sizeof(i_ccon_cpy), compare_contact_cpy_index, print_error};
    t_datablock index_block_con = {&db->hdr.off_i_con_cpy, &db->hdr.i_con_cpy, sizeof(i_ccon_cpy), assign_contact_index_slot, assign_contact_index_cpy};
    t_datablock table_block_con = {&db->hdr.off_con, 0, sizeof(ccon), NULL, NULL};
    //campaign metadata
    meta_t index_cam = {NULL, NULL, db->hdr.nr_cam, sizeof(i_ccam_PK), compare_campaign_PK_index, print_error};
    t_datablock index_block_cam = {&db->hdr.off_i_cam_pk, &db->hdr.i_cam_pk, sizeof(i_ccam_PK), assign_campaign_index_slot, assign_campaign_index_PK};
    t_datablock table_block_cam = {&db->hdr.off_cam, 0, sizeof(ccam), NULL, NULL};
    //country metadata
    meta_t index_cty = {NULL, NULL, db->hdr.nr_cty, sizeof(i_ccty_name), compare_country_index_name, print_error};
    t_datablock index_block_cty = {&db->hdr.off_i_cty_name, &db->hdr.i_cty_name, sizeof(i_ccty_name), assign_contact_index_slot, assign_country_index_name};
    t_datablock table_block_cty = {&db->hdr.off_cty, 0, sizeof(ccty), NULL, NULL};
    //group FK metadata
    meta_t index_grp_fk = {NULL, NULL, db->hdr.nr_grp, sizeof(i_cgrp_FK), compare_group_FK_index, print_error};
    t_datablock index_block_grp_fk = {&db->hdr.off_i_grp_fk, &db->hdr.i_grp_fk, sizeof(i_cgrp_FK), assign_group_FK_index_slot, assign_group_index_FK};
    t_datablock table_block_grp = {&db->hdr.off_grp, 0, sizeof(cgrp), NULL, NULL};
    //group name metadata
    meta_t index_grp_nm = {NULL, NULL, db->hdr.nr_grp, sizeof(i_cgrp_nm), compare_group_nm_index_char, print_error};
    t_datablock index_block_grp_nm = {&db->hdr.off_i_grp_nm, &db->hdr.i_grp_nm, sizeof(i_cgrp_nm), assign_group_nm_index_slot, assign_group_index_nm};
    //industry metadata
    meta_t index_ind = {NULL, NULL, db->hdr.nr_ind, sizeof(i_cind_PK), compare_industry_PK_index, print_error};
    t_datablock index_block_ind = {&db->hdr.off_i_ind_pk, &db->hdr.i_ind_pk, sizeof(i_cind_PK), assign_industry_index_slot, assign_industry_index_PK};
    t_datablock table_block_ind = {&db->hdr.off_ind, 0, sizeof(cind), NULL, NULL};
    //job metadata
    meta_t index_job = {NULL, NULL, db->hdr.nr_job, sizeof(i_cjob_name), compare_job_index_name, print_error};
    t_datablock index_block_job = {&db->hdr.off_i_job_name, &db->hdr.i_job_name, sizeof(i_cjob_name), assign_job_index_slot, assign_job_index_name};
    t_datablock table_block_job = {&db->hdr.off_job, 0, sizeof(cjob), NULL, NULL};

    //attempt creating the DB file
    if(Create_DB(db, DB_name) < 0){
        fprintf(stderr, "create_db() : Error while creating %s\n", DB_name);
        return;
    }

    db->hdr.nr_cpy = Import_CSV(db, CSV_cpy_imp, db->hdr.off_cpy, sizeof(ccpy), CSVDeserialiseCompany);
    db->hdr.nr_cam = Import_CSV(db, CSV_cam_imp, db->hdr.off_cam, sizeof(ccam), CSVDeserialiseCampaign);
    db->hdr.nr_con = Import_CSV(db, CSV_con_imp, db->hdr.off_con, sizeof(ccon), CSVDeserialiseContact);

    Import_CSV_Country(db);
    Import_CSV_Group(db);
    Import_CSV_industry(db);
    Import_CSV_job(db);

    printf("Creating the index for Companies (by name) : ");
    create_index_file(db, &index_cpy_nm, db->hdr.nr_cpy, &index_block_cpy_nm, &table_block_cpy);
    printf("done\n");
    printf("Creating the index for Companies (by group ID) : ");
    create_index_file(db, &index_cpy_grp, db->hdr.nr_cpy, &index_block_cpy_grp, &table_block_cpy);
    printf("done\n");
    printf("Creating the index for Contacts (by company FK) : ");
    create_index_file(db, &index_con, db->hdr.nr_con, &index_block_con, &table_block_con);
    printf("done\n");
    printf("Creating the index for Campaigns (by ID) : ");
    create_index_file(db, &index_cam, db->hdr.nr_cam, &index_block_cam, &table_block_cam);
    printf("done\n");
    printf("Creating the index for Countries (by name) : ");
    create_index_file(db, &index_cty, db->hdr.nr_cty, &index_block_cty, &table_block_cty);
    printf("done\n");
    printf("Creating the index for Groups (by FK) : ");
    create_index_file(db, &index_grp_fk, db->hdr.nr_grp, &index_block_grp_fk, &table_block_grp);
    printf("done\n");
    printf("Creating the index for Groups (by name) : ");
    create_index_file(db, &index_grp_nm, db->hdr.nr_grp, &index_block_grp_nm, &table_block_grp);
    printf("done\n");
    printf("Creating the index for Industries (by ID) : ");
    create_index_file(db, &index_ind, db->hdr.nr_ind, &index_block_ind, &table_block_ind);
    printf("done\n");
    printf("Creating the index for Jobs (by name) : ");
    create_index_file(db, &index_job, db->hdr.nr_job, &index_block_job, &table_block_job);
    printf("done\n");
}

/****************************************************************************************/
/*  I : Database to load in memory                                                      */
/*  P : Reads the whole database and creates arrays containing the data                 */
/*  O : /                                                                               */
/****************************************************************************************/
int load_db(dbc* db){
    meta_t table_cty = {NULL, NULL, 0, sizeof(ccty), NULL, NULL};
    meta_t table_cpy = {NULL, NULL, 0, sizeof(ccpy), NULL, NULL};
    meta_t table_cam = {NULL, NULL, 0, sizeof(ccam), NULL, NULL};
    meta_t table_con = {NULL, NULL, 0, sizeof(ccon), NULL, NULL};
    meta_t table_grp = {NULL, NULL, 0, sizeof(cgrp), NULL, NULL};
    meta_t table_ind = {NULL, NULL, 0, sizeof(cind), NULL, NULL};
    meta_t table_job = {NULL, NULL, 0, sizeof(cjob), NULL, NULL};


    //open the files and position the pointers at the end
    db->fp = fopen(DB_file, "rb");
    if(!db->fp)
        return -1;

    fseek(db->fp, 0, SEEK_SET);
    fread(&db->hdr, sizeof(db->hdr), 1, db->fp);
    fclose(db->fp);

    system("cls");

    printf("Loading countries : ");
    table_cty.nbelements = db->hdr.nr_cty;
    Load_table(db, &table_cty, db->hdr.off_cty);
    db->cty = (ccty*)table_cty.structure;
    printf("done\n");

    printf("Loading companies : ");
    table_cpy.nbelements = db->hdr.nr_cpy;
    Load_table(db, &table_cpy, db->hdr.off_cpy);
    db->cpy = (ccpy*)table_cpy.structure;
    printf("done\n");

    printf("Loading campaigns : ");
    table_cam.nbelements = db->hdr.nr_cam;
    Load_table(db, &table_cam, db->hdr.off_cam);
    db->cam = (ccam*)table_cam.structure;
    printf("done\n");

    printf("Loading contacts : ");
    table_con.nbelements = db->hdr.nr_con;
    Load_table(db, &table_con, db->hdr.off_con);
    db->con = (ccon*)table_con.structure;
    printf("done\n");

    printf("Loading groups : ");
    table_grp.nbelements = db->hdr.nr_grp;
    Load_table(db, &table_grp, db->hdr.off_grp);
    db->grp = (cgrp*)table_grp.structure;
    printf("done\n");

    printf("Loading industries: ");
    table_ind.nbelements = db->hdr.nr_ind;
    Load_table(db, &table_ind, db->hdr.off_ind);
    db->ind = (cind*)table_ind.structure;
    printf("done\n");

    printf("Loading jobs : ");
    table_job.nbelements = db->hdr.nr_job;
    Load_table(db, &table_job, db->hdr.off_job);
    db->job = (cjob*)table_job.structure;
    printf("done\n\n");

    system("pause");

    return 0;
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
    meta_t cty_list = {NULL, NULL, 0, sizeof(ccty), compare_country_name, print_error};
    meta_t cty_array = {NULL, NULL, db->hdr.nr_cty, sizeof(ccty), NULL, print_error};
    t_datablock cty_data = {&db->hdr.off_cty, NULL, sizeof(ccty), NULL, NULL};
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
                foreachList(&cty_list, NULL, Rec_Country);
                break;

            case '1':
                Export_CSV(db, CSV_cty_exp, CSV_cty_header, &cty_data, cty_array.nbelements, CSVFormatCountry);
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
    uint64_t j = 0;
    dyndata_t *companies = NULL;
    meta_t cpy_list = {NULL, NULL, 0, sizeof(ccpy), compare_company_name, print_error};
    meta_t cpy_array = {NULL, NULL, db->hdr.nr_cpy, sizeof(ccpy), NULL, print_error};
    t_datablock cpy_data = {&db->hdr.off_cpy, NULL, sizeof(ccpy), NULL, NULL};
    char menu_cpy[4][32]={  "Menu des Compagnies",
                            "Lister les Compagnies",
                            "Exporter les Compagnies",
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
                        Rec_company(companies->data, NULL);
                        companies = getright(companies);
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
                Export_CSV(db, CSV_cpy_exp, CSV_cpy_header, &cpy_data, cpy_array.nbelements, CSVFormatCompany);
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
    uint64_t j = 0;
    dyndata_t *campaign = NULL;
    meta_t cam_list = {NULL, NULL, 0, sizeof(ccam), compare_campaign_PK, print_error};
    meta_t cam_array = {NULL, NULL, db->hdr.nr_cam, sizeof(ccam), NULL, print_error};
    t_datablock cam_data = {&db->hdr.off_cam, NULL, sizeof(ccam), NULL, NULL};
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
                        Rec_campaign(campaign->data, NULL);
                        campaign = getright(campaign);
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
                Export_CSV(db, CSV_cam_exp, CSV_cam_header, &cam_data, cam_array.nbelements, CSVFormatCampaign);
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
    uint64_t j = 0;
    dyndata_t *contact = NULL;
    meta_t con_list = {NULL, NULL, 0, sizeof(ccon), compare_contact_cpy, print_error};
    meta_t con_array = {NULL, NULL, db->hdr.nr_con, sizeof(ccon), NULL, print_error};
    t_datablock con_data = {&db->hdr.off_con, NULL, sizeof(ccon), NULL, NULL};
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
                        Rec_contact(contact->data, NULL);
                        contact = getright(contact);
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
                Export_CSV(db, CSV_con_exp, CSV_con_header, &con_data, con_array.nbelements, CSVFormatContact);
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
    uint64_t j = 0;
    dyndata_t *group = NULL;
    meta_t grp_list = {NULL, NULL, 0, sizeof(cgrp), compare_group_FK, print_error};
    meta_t grp_array = {NULL, NULL, db->hdr.nr_grp, sizeof(cgrp), NULL, print_error};
    t_datablock grp_data = {&db->hdr.off_grp, NULL, sizeof(cgrp), NULL, NULL};
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
                        Rec_Group(group->data, NULL);
                        group = getright(group);
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
                Export_CSV(db, CSV_grp_exp, CSV_grp_header, &grp_data, grp_array.nbelements, CSVFormatGroup);
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
    uint64_t j = 0;
    dyndata_t *industry = NULL;
    meta_t ind_list = {NULL, NULL, 0, sizeof(cind), compare_industry_PK, print_error};
    meta_t ind_array = {NULL, NULL, db->hdr.nr_ind, sizeof(cind), NULL, print_error};
    t_datablock ind_data = {&db->hdr.off_ind, NULL, sizeof(cind), NULL, NULL};
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
                        Rec_industry(industry->data, NULL);
                        industry = getright(industry);
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
                Export_CSV(db, CSV_ind_exp, CSV_ind_header, &ind_data, ind_array.nbelements, CSVFormatIndustry);
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
    uint64_t j = 0;
    dyndata_t *job = NULL;
    meta_t job_list = {NULL, NULL, 0, sizeof(cjob), compare_job_name, print_error};
    meta_t job_array = {NULL, NULL, db->hdr.nr_job, sizeof(cjob), NULL, print_error};
    t_datablock job_data = {&db->hdr.off_job, NULL, sizeof(cjob), NULL, NULL};
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
                        Rec_job(job->data, NULL);
                        job = getright(job);
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
                Export_CSV(db, CSV_job_exp, CSV_job_header, &job_data, job_array.nbelements, CSVFormatJob);
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

/************************************************************/
/*  I : /                                                   */
/*  P : Generates and exports the appointed screen report   */
/*  O : -1 if error                                         */
/*       0 otherwise                                        */
/************************************************************/
int gen_detailed_report(dbc* db){
    char com[64] = "0";

    system("cls");
    printf("****************************************\n");
    printf("*           Rapport detaille :         *\n");
    printf("*  Contacts pour une compagnie, tries  *\n");
    printf("*         par type de campagne         *\n");
    printf("****************************************\n");

    printf("\n\nVeuillez taper le nom d'un groupe : ");
    fgets(com, sizeof(com), stdin);
    if ((strlen(com) > 0) && (com[strlen(com)-1] == '\n'))
        com[strlen(com)-1] = '\0';

    printf("\n\nGeneration du rapport\n");
    export_detailed_report(db, com);

    return 0;
}

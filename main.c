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

char DB_name[32] = "DB_Comp";

void init_db(dbc* db);
char menu(int, char[][32]);

int main(int argc, char *argv[])
{
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
    dbc db = {0};

    init_db(&db);

    do{
        choice = menu(sizeof(princ_menu)/32, princ_menu);
        switch(choice){
            case '0':   //Countries menu
                break;

            case '1':   //Companies menu
                break;

            case '2':   //Campaigns menu
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
        printf("\nAppuyez sur une touche pour continuer ");
        fflush(stdin);
        getch();
    }while(choice!=27);

    if(db.cpy)
        free(db.cpy);
    if(db.con)
        free(db.con);

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
    //campain metadata
    t_algo_meta index_cam = {NULL, db->nr_cam, sizeof(i_ccam_PK), compare_campaign_PK_index, swap_campaign_index, assign_campaign_index_PK, assign_campaign_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block_cam = {&db->hdr.off_i_cam_pk, &db->hdr.i_cam_pk, sizeof(i_ccam_PK)};
    t_datablock table_block_cam = {&db->hdr.off_cam, 0, sizeof(ccam)};

    Create_DB(db, DB_name);
    Import_CSV_company(db);
    Import_CSV_contact(db);
    Import_CSV_campaign(db);

    create_index_file(db, &index_cpy, db->nr_cpy, &index_block_cpy, &table_block_cpy);
    create_index_file(db, &index_con, db->nr_con, &index_block_con, &table_block_con);
    create_index_file(db, &index_cam, db->nr_cam, &index_block_cam, &table_block_cam);
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

    return choice;
}

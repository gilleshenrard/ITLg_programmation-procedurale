#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

char DB_name[32] = "DB_Comp";

void init_db(dbc* db);
int print_screen_report(dbc* db, char* nm_cpy);

int main(int argc, char *argv[])
{
    dbc db = {0};

    init_db(&db);

    print_screen_report(&db, "Ernst & Young Management Consultants");

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

    Create_DB(db, DB_name);
    Import_CSV_company(db);
    Import_CSV_contact(db);

    create_index_file(db, &index_cpy, db->nr_cpy, &index_block_cpy, &table_block_cpy);
    create_index_file(db, &index_con, db->nr_con, &index_block_con, &table_block_con);
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
        printf("\nVeuillez choisir pour quelle compagnie afficher le rapport : (1 a %d)\n\n", list_cpy.nbelements);
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

    fclose(db->fp);

    return 0;
}

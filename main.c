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

    return EXIT_SUCCESS;
}

/****************************************************************************************/
/*  I : Database to create                                                              */
/*  P : Creates a database file and initializes it with the required tables and indexes */
/*  O : /                                                                               */
/****************************************************************************************/
void init_db(dbc* db){
    t_algo_meta index = {NULL, db->nr_cpy, sizeof(i_ccpy_name), compare_company_index_name, swap_company_index, assign_company_index_name, assign_company_index_slot, NULL, NULL, NULL, NULL};
    t_datablock index_block={&db->hdr.off_i_cpy_name, &db->hdr.i_cpy_name, sizeof(i_ccpy_name)};
    t_datablock table_block={&db->hdr.off_cpy, 0, sizeof(ccpy)};

    Create_DB(db, DB_name);
    Import_CSV_company(db);

    create_index_file(db, &index, db->nr_cpy, &index_block, &table_block);
}

/****************************************************************************************/
/*  I : Database from which print the screen report                                     */
/*  P : Creates and prints a report                                                     */
/*          report : Contacts for a company, sorted by campaign type                    */
/*  O : -1 if not found                                                                 */
/*       0 otherwise                                                                    */
/****************************************************************************************/
int print_screen_report(dbc* db, char* nm_cpy){
    t_algo_meta index = {0};
    t_algo_meta cpy_list = {NULL, 0, sizeof(ccpy_recur), compare_company_name, swap_company, assign_company, NULL, NULL, NULL, company_right, company_left};
    int choix = 0;
    char buffer[6] = {0};
    ccpy_recur* cpy_buffer = NULL, *next=NULL;

    index.elementsize = sizeof(i_ccpy_name);
    index.doCompare = compare_company_index_char;

    // open the DB and search for all occurrences of the company name (creates a linked list)
    db->fp = fopen(DB_file, "rb");
    if(db->fp){
        searchall_index(db->fp, db->hdr.i_cpy_name, nm_cpy, &index, &cpy_list, sizeof(ccpy));
        cpy_buffer=cpy_list.structure;
        fclose(db->fp);
    }
    else{
        fprintf(stderr, "\n\nErreur a l'ouverture de la DB\n\n");
        return -1;
    }

    //if not found, error
    if(!cpy_list.nbelements){
        fprintf(stderr, "\n\n%s : non-trouve\n\n", nm_cpy);
        return -1;
    }

    //if more than one occurrence, make the user choose which one
    if(cpy_list.nbelements > 1){
        printf("\nVeuillez choisir pour quelle compagnie afficher le rapport : (1 a %d)\n\n", cpy_list.nbelements);
        foreachList(&cpy_list, NULL, Rec_company_list);

        printf("\nChoix : ");
        fflush(stdin);
        fgets(buffer, 6, stdin);
        choix = atoi(buffer);

        next = cpy_list.structure;
        for(int i=1 ; i<choix ; i++){
            next = *company_right(next);
        }
        cpy_buffer = (ccpy_recur*)next;
    }

    Rec_company_list(cpy_buffer, NULL);

    while(cpy_list.structure)
        popListTop(&cpy_list);

    return 0;
}

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

void init_db(dbc*);
void tst_export_country(dbc*);
void tst_Load_country(dbc*);
void tst_Print_country(dbc*);
void tst_List_country(dbc*);
void tst_AVL_country(dbc*);
void tst_AVL_search_country(dbc*);
void tst_index_country_name(dbc* db);

/****************************************************************************************
* Programme principal
****************************************************************************************/
int main(void)
{
    dbc db;
    init_db(&db);

//    tst_export_country(&db);
    tst_Load_country(&db);
//    tst_Print_country(&db);
//    tst_List_country(&db);
    tst_AVL_country(&db);
//    tst_AVL_search_country(&db);
    tst_index_country_name(&db);

	return 0;
}

/****************************************************************************************/
/*  I : Database to create and populate                                                 */
/*  P : Creates the database files and populates it (export) with the files             */
/*          located in import/                                                          */
/*  O : /                                                                               */
/****************************************************************************************/
void init_db(dbc* db){
    Create_DB(db);
    Import_CSV_Country(db);

    printf("header size : %d\n", sizeof(hder));
    printf("ccty size : %d\n", sizeof(ccty));
    printf("cjob size : %d\n", sizeof(cjob));
    printf("cind size : %d\n", sizeof(cind));
    printf("cgrp size : %d\n", sizeof(cgrp));
    printf("i_ccty_name size : %d\n", sizeof(i_ccty_name));
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
    t_algo_meta cty_list = {NULL, 0, sizeof(ccty_recur), compare_country_name, swap_country, assign_country, NULL, NULL, country_right, country_left};
    t_algo_meta cty_array = {db->cty+1, db->nr_cty, sizeof(ccty), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

    printf("\n--------------- tst_list_insert ---------------------------------------\n");
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
    t_algo_meta cty_avl = {NULL, 0, sizeof(ccty_recur), compare_country_name, swap_country, assign_country, get_country_height, set_country_height, country_right, country_left};
    t_algo_meta cty_array = {db->cty+1, db->nr_cty, sizeof(ccty), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    ccty_recur tmp = {db->cty[3], 0, NULL, NULL};

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
    t_algo_meta cty_avl = {NULL, 0, sizeof(ccty_recur), compare_country_name, swap_country, assign_country, get_country_height, set_country_height, country_right, country_left};
    t_algo_meta cty_array = {db->cty+1, db->nr_cty, sizeof(ccty), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
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
    FILE* fp = NULL;
    i_ccty_name buffer = {0};

    printf("\n--------------- tst_index_country_name ---------------------------------\n");

    create_index_unbuffered(db, compare_country_name);

    printf("Number of elements : %d\n", db->nr_cty);
    printf("index block offset : %X\n", db->hdr.off_i_cty_name);
    printf("index tree root : %X\n\n", db->hdr.i_cty_name);

    fp = fopen(DB_file, "rb");
    if(fp){
        printf("\t\t\tNAME\tOFFSET\tLEFT\tRIGHT\n\n");
        fseek(fp, db->hdr.off_i_cty_name, SEEK_SET);
        for(int i=0 ; i<db->nr_cty ; i++){
            fread(&buffer, sizeof(i_ccty_name), 1, fp);
            printf("%28s\t%lx\t%lx\t%lx\n", buffer.nm_cty, ftell(fp), buffer.s_left, buffer.s_right);
        }
        fclose(fp);
    }
}

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

void init_db(dbc*);
void tst_export_country(dbc*);
void tst_Load_country(dbc*);
void tst_Print_country(dbc*);
void tst_List_country(dbc*);
void tst_AVL_country(dbc*);
void tst_AVL_search_country(dbc*);

/****************************************************************************************
* Programme principal
****************************************************************************************/
int main(void)
{
    dbc db;
    init_db(&db);

    tst_export_country(&db);
    tst_Load_country(&db);
    tst_Print_country(&db);
    tst_List_country(&db);
    tst_AVL_country(&db);
    tst_AVL_search_country(&db);

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
    t_algo_meta cty_list = {NULL, 0, sizeof(ccty_recur), allocate_country, compare_country_name, swap_country, assign_country, NULL, NULL, country_right, country_left};
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
    t_algo_meta cty_avl = {NULL, 0, sizeof(ccty_recur), allocate_country, compare_country_name, swap_country, assign_country, get_country_height, set_country_height, country_right, country_left};
    t_algo_meta cty_array = {db->cty+1, db->nr_cty, sizeof(ccty), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    ccty_recur tmp = {db->cty[3], 0, NULL, NULL};

    printf("\n--------------- tst_AVL_insert ---------------------------------------\n");

    //prepare the AVL tree from the countries array
    arrayToAVL(&cty_array, &cty_avl, COPY);
    display_AVL_tree(&cty_avl, cty_avl.structure, 'R', &toString_Country);
    printf("\n\n");

    //try to add an already existing node
    printf("Trying to add an already existing country (forbidden)\n");
    printf("%d countries before\n", cty_avl.nbelements);
    insertAVL(&cty_avl, cty_avl.structure, &tmp);
    printf("%d countries after\n\n", cty_avl.nbelements);

    //display the countries as a list
    foreachAVL(&cty_avl, cty_avl.structure, NULL, Rec_Country_list);

    //free memory
    foreachAVL(&cty_avl, cty_avl.structure, NULL, free_country);
}

/****************************************************************************************/
/*  I : Country Database in which look for a key                                        */
/*  P : Tests the AVL key search method                                                 */
/*  O : /                                                                               */
/****************************************************************************************/
void tst_AVL_search_country(dbc* db){
    t_algo_meta cty_avl = {NULL, 0, sizeof(ccty_recur), allocate_country, compare_country_name, swap_country, assign_country, get_country_height, set_country_height, country_right, country_left};
    t_algo_meta cty_array = {db->cty+1, db->nr_cty, sizeof(ccty), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char countries[5][16] = {"Denmark", "Portugal", "Burkina Faso", "Estonia", "Singapore"};

    printf("\n--------------- tst_AVL_search_country ---------------------------------\n");
    arrayToAVL(&cty_array, &cty_avl, COPY);
    cty_avl.doCompare = compare_country_name_char;
    for(int i=0 ; i<5 ; i++){
        printf("Searching for %s\t\t: ", countries[i]);
        if (search_AVL(&cty_avl, cty_avl.structure, countries[i]))
            printf("found\n");
        else
            printf("not found\n");
    }
    foreachAVL(&cty_avl, cty_avl.structure, NULL, free_country);
}

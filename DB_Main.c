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
    t_algo_meta cty_list = {NULL, 0, sizeof(ccty_recur), compare_country_name, swap_country, assign_country, country_right, country_left};
    t_algo_meta cty_array = {db->cty, db->nr_cty, sizeof(ccty), NULL, NULL, NULL, NULL, NULL};

    printf("\n--------------- tst_list_country --------------------------------------\n");
    arrayToList(&cty_array, &cty_list, COPY);
    foreachList(&cty_list, NULL, Rec_Country_list);
    while(cty_list.structure)
        popListTop(&cty_list);
}

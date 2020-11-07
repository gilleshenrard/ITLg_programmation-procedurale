/****************************************************************************************
* Dossier 2 : Database "Analyse de donnees clients"
*
* Manipulations de la table country : fichiers non geres globalement
*
* Programmation procedurale 2019 - E.Bosly - Version 0
****************************************************************************************/
#ifndef DB_COUNTRY_H_INCLUDED
#define DB_COUNTRY_H_INCLUDED
#include "DB_Main.h"
#include "DB_File.h"

#define PRT 0
#define BUF_LEN 200

#define CSV_cty_imp "Data_Import/DB_Country.csv"
#define CSV_cty_exp "Data_Export/DB_Country.csv"
#define CSV_cty_header "Id;Nm_Cty;Nm_Zon;Cd_Iso\n"

typedef struct i_Country_Name{
    char tp_rec[SZ_TYPE];   //record type (I_CTY)
    char nm_cty[SZ_NAME];   //Country name
    char filler[16];        //filler to get the index size to 64
    long slot;              //Slot number in the table
    long s_left;            //Slot number of the left child in the table
    long s_right;           //Slot number of the right child in the table
}i_ccty_name;

//database methods
void Import_CSV_Country(dbc *db);
int CSVFormatCountry(void* elem, char* finalLine);
void Load_Country(dbc *db);
void Print_Country(dbc *db);
int Rec_Country(void *rec, void* nullable);

// dynamic structures methods
int compare_country_name(void* a, void* b);
int compare_country_index_name(void* a, void* b);
int assign_country_index_name(void* index, void* elem);
int assign_country_index_slot(void* index, uint32_t* offset);
char* toString_Country(void* current);

//file structures methods

#endif // DB_COUNTRY_H_INCLUDED

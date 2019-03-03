/****************************************************************************************
* Dossier 2 : Database "Analyse de donnees clients"
*
* Manipulations de la table country : fichiers non geres globalement
*
* Programmation procedurale 2019 - E.Bosly - Version 0
****************************************************************************************/
#ifndef DB_COUNTRY_H_INCLUDED
#define DB_COUNTRY_H_INCLUDED
#include "../lib/DB_Main.h"

#define PRT 0
#define BUF_LEN 200

typedef struct ccty_recur ccty_recur;

struct ccty_recur{
    ccty cty;
    int height;
    ccty_recur *left;
    ccty_recur *right;
};

//database methods
void Import_CSV_Country(dbc *db);
void Export_CSV_Country(dbc *db);
void Load_Country(dbc *db);
void Print_Country(dbc *db);
void Rec_Country(ccty *rec);

// dynamic lists methods
void* allocate_country(void);
int compare_country_name(void* a, void* b);
int compare_country_name_char(void* a, void* b);
int assign_country(void* oldelem, void* newelem);
int swap_country(void* first, void* second);
void** country_right(void* current);
void** country_left(void* current);
int Rec_Country_list(void *record, void* nullable);
char* toString_Country(void* current);
int get_country_height(void* current);
int set_country_height(void* current, int value);
int free_country(void* country, void* nullable);

#endif // DB_COUNTRY_H_INCLUDED

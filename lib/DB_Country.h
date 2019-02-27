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

void Import_CSV_Country(dbc *db);
void Export_CSV_Country(dbc *db);
void Load_Country(dbc *db);
void Print_Country(dbc *db);
void Rec_Country(dbc *db, int id_cty);

#endif // DB_COUNTRY_H_INCLUDED

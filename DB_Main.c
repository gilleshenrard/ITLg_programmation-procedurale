/****************************************************************************************
* Dossier 2 : Database "Analyse de donnees clients"
*
* Test des routines de demo : fichiers non geres globalement
*
* Programmation procedurale 2019 - E.Bosly - Version 0
****************************************************************************************/
#include "lib/DB_Main.h"

/****************************************************************************************
* Programme principal
****************************************************************************************/
int main(void)
{
    dbc db;

    Create_DB(&db);

    Import_CSV_Country(&db);

    Export_CSV_Country(&db);

    Load_Country(&db);

    Print_Country(&db);

	return 0;
}

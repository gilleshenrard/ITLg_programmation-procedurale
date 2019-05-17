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


int main(int argc, char *argv[])
{
    dbc db = {0};

    Create_DB(&db, "DB_Comp");

    return EXIT_SUCCESS;
}

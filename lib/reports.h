#ifndef REPORTS_H_INCLUDED
#define REPORTS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "DB_Main.h"
#include "DB_Country.h"
#include "algo.h"
#include "DB_File.h"
#include "DB_Group.h"
#include "DB_Job.h"
#include "DB_Industry.h"
#include "DB_campaign.h"
#include "DB_Contact.h"
#include "DB_Company.h"

int print_screen_report(dbc* db, char* nm_cpy);

#endif // REPORTS_H_INCLUDED

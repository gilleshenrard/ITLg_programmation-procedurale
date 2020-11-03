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
#include "screen.h"

typedef struct cscr_recur cscr_recur;
typedef struct cdtl_recur cdtl_recur;

typedef struct scr_report{
    int id_job;
    int id_cam;
    char cam_nm[SZ_NAME];
    char cam_tp[SZ_NAME];
    int nr_rep;
}cscr;

typedef struct dtl_report{
    char nm_zone[SZ_NAME_SM];
    ccpy_recur* companies;
}cdtl;

struct cdtl_recur{
    cdtl rep;
    int height;
    cdtl_recur *left;
    cdtl_recur *right;
};

// SCREEN REPORT METHODS

int compare_scr_report_type(void* a, void* b);
int Rec_scr_report(void *rec, void *nullable);

// REPORT GENERATION METHODS

int print_screen_report(dbc* db, char* nm_cpy);
int export_aggregated_report(dbc* db);
int export_detailed_report(dbc* db, char* nm_grp);

#endif // REPORTS_H_INCLUDED

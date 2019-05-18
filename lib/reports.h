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

typedef struct cscr_recur cscr_recur;

typedef struct scr_report{
    int id_job;
    int id_cam;
    char cam_nm[SZ_NAME];
    char cam_tp[SZ_NAME];
    int nr_rep;
}cscr;

struct cscr_recur{
    cscr rep;
    int height;
    cscr_recur *left;
    cscr_recur *right;
};

void* allocate_scr_report(void);
int compare_scr_report_name(void* a, void* b);
int compare_scr_report_name_char(void* a, void* b);
int copy_scr_report(void* oldelem, void* newelem);
int swap_scr_report(void* first, void* second);
void** scr_report_right(void* current);
void** scr_report_left(void* current);
int Rec_scr_report_list(void *record, void* nullable);
char* toString_scr_report(void* current);
int get_scr_report_height(void* current);
int set_scr_report_height(void* current, int value);
void* free_scr_report(void* report, void* nullable);


int print_screen_report(dbc* db, char* nm_cpy);

#endif // REPORTS_H_INCLUDED

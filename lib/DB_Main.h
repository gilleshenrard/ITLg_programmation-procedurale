/****************************************************************************************
* Dossier 2 : Database "Analyse de donnees clients"
*
* Definition des structures : exemple preliminaire
*
* Programmation procedurale 2019 - E.Bosly - Version 0
****************************************************************************************/
#ifndef DB_MAIN_H_INCLUDED
#define DB_MAIN_H_INCLUDED
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SZ_CTY 100
#define SZ_JOB 200
#define SZ_IND 100
#define SZ_GRP 3000
#define SZ_CAM 600
#define SZ_CON 25000000
#define SZ_CPY 100000

#define SZ_TYPE 8
#define SZ_NAME 28
#define SZ_NAME_SM 20

#define DB_file "Data_DB_Comp\\DB_Comp.dat"
#define log_file "Data_DB_Comp\\DB_Comp.log"

typedef unsigned int uint;
typedef unsigned short usint;

/***************************************************************************************
* Header de la database
****************************************************************************************/
typedef struct Header
{
    uint        db_size;           // Taille de la database
    char        db_name[SZ_NAME];  // Nom de la database
    uint        sz_cty;            // Taille bloc country
    uint        sz_job;            // Taille bloc job
    uint        sz_ind;            // Taille bloc industry
    uint        sz_grp;            // Taille bloc group
    uint        sz_cam;            // Taille bloc campain
    uint32_t    sz_con;            // Taille bloc contact
    uint32_t    sz_cpy;            // Taille bloc company
    uint32_t    off_cty;           // Position bloc country
    uint32_t    off_job;           // Position bloc job
    uint32_t    off_ind;           // Position bloc industry
    uint32_t    off_grp;           // Position bloc group
    uint32_t    off_cam;           // Position bloc campain
    uint32_t    off_con;           // Position bloc contact
    uint32_t    off_cpy;           // Position bloc company
    uint32_t    off_i_cty_name;    // Offset of the country index block (by name)
    uint32_t    off_i_grp_fk;      // Offset of the group index block (by fk)
    uint32_t    off_i_job_name;    // Offset of the job index block (by name)
    uint32_t    off_i_ind_pk;      // Offset of the industry index block (by pk)
    uint32_t    off_i_cam_pk;      // Offset of the campain index block (by pk)
    uint32_t    off_i_con_cpy;     // Offset of the contact index block (by campains)
    uint32_t    off_i_cpy_name;    // Offset of the company index block (by name)
    uint32_t    off_i_cpy_grp;     // Offset of the company index block (by group ID)
    uint32_t    off_i_grp_nm;      // Offset of the group index block (by name)
    uint32_t    i_cty_name;        // Address of the root for the country index (by name)
    uint32_t    i_grp_fk;          // Address of the root for the group index (by fk)
    uint32_t    i_job_name;        // Address of the root for the job index (by name)
    uint32_t    i_ind_pk;          // Address of the root for the industry index (by pk)
    uint32_t    i_cam_pk;          // Address of the root for the campain index (by pk)
    uint32_t    i_con_cpy;         // Address of the root for the contact index (by campains)
    uint32_t    i_cpy_name;        // Address of the root for the company index (by name)
    uint32_t    i_cpy_grp;         // Address of the root for the company index (by group)
    uint32_t    i_grp_nm;          // Address of the root for the group index (by name)
    char        filler[96];        //get the size of hder to 256 bits
} hder;

/***************************************************************************************
* Table Country
****************************************************************************************/
typedef struct Country
{
    char tp_rec[SZ_TYPE];   // Type de record CTY
    int  id_cty;            // Cle primaire
    char nm_zon[SZ_NAME_SM];        // Nom de la zone geographique
    char nm_cty[SZ_NAME];   // Nom du pays
    char cd_iso[4];         // Code Iso du pays
} ccty;

/***************************************************************************************
* Table Job : dummy
****************************************************************************************/
typedef struct Job
{
    char tp_rec[SZ_TYPE];   // Type de record JOB
    int  id_job;            // Cle primaire
    char nm_lev[SZ_NAME];
    char nm_dep[SZ_NAME];
    char nm_job[SZ_NAME];
    char filler[32];
} cjob;

/***************************************************************************************
* Table Industry : dummy
****************************************************************************************/
typedef struct Industry
{
    char tp_rec[SZ_TYPE];   // Type de record IND
    int  id_ind;            // Cle primaire
    char nm_sec[SZ_NAME_SM];
    char nm_ind[SZ_NAME_SM];
    char filler[12];
} cind;

/***************************************************************************************
* Table Group : dummy
****************************************************************************************/
typedef struct Group
{
    char tp_rec[SZ_TYPE];   // Type de record GRP
    int  id_grp;            // Cle primaire
    char nm_grp[SZ_NAME];
    char cd_cty[4];         // Group country tag
    int  id_cty;             // Foreign Key
    char filler[16];        // Filler to bring the group record size to 64 bytes
} cgrp;

/***************************************************************************************
* Table Campain : dummy
****************************************************************************************/
typedef struct Campain
{
    char  tp_rec[SZ_TYPE];   // Type de record CAM
    int   id_cam;            // Cle primaire
    char  nm_cam[SZ_NAME];   // Campain name
    char  tp_cam[SZ_NAME];   // Campain type
    char  dt_cam[11];        // Campain start date
    char  nm_lev[SZ_NAME];   // Campain level denomination
    char  nm_dep[SZ_NAME];   // Campain relevant department name
    char  nm_sec[SZ_NAME];   // Campain relevant sector name
    char  nm_zon[SZ_NAME];   // Campain targeted zone
    int   nr_year;           // Amount of years the campain lasts
    float cost;              // Cost per year ?
    char  filler[56];        // Filler to bring the group record size to 64 bytes
} ccam;

/***************************************************************************************
* Table Contact : dummy
****************************************************************************************/
typedef struct Contact
{
    char  tp_rec[SZ_TYPE];   // Type de record CON
    int   id_cam;            // FK Campain
    int   id_cpy;            // FK Company
    int   id_job;            // FK Job
    int   nr_rep;            // Number of replies ?
    char  filler[8];        // Filler to bring the group record size to 64 bytes
} ccon;

/***************************************************************************************
* Table Company : dummy
****************************************************************************************/
typedef struct Company
{
    char  tp_rec[SZ_TYPE];   // Type de record CPY
    int   id_cpy;            // PK company ID
    int   id_cty;            // FK Country ID
    int   id_ind;            // FK Industry ID
    int   id_grp;            // FK Group ID
    char  nm_cpy[64];        // Company name
    char  nm_adr[64];        // Company address
    char  nm_cit[SZ_NAME-4]; // Company city
    char  cd_pos[10];        // company postcode
    char  nr_tel[SZ_NAME];   // company phone number
    char  nm_www[SZ_NAME];   // company website
    char  dt_cre[11];        // company foundation date
} ccpy;

/***************************************************************************************
* Structure base de donnees
****************************************************************************************/
typedef struct	db_country
{
    FILE*       fp;     // File pointer to the database
    hder        hdr;    // Header
    ccty*       cty;    // Buffer Country
    cgrp*       grp;    // Buffer Group
    cjob*       job;    // Buffer Job
    cind*       ind;    // Buffer industry
    ccam*       cam;    // Buffer campain
    ccon*       con;    // Buffer contacts
    ccpy*       cpy;    // Buffer contacts
    uint32_t    nr_cty; // Nr elements dans buffer
    uint32_t    nr_grp; // Nr Groups in the buffer
    uint32_t    nr_job; // Nr Jobs in the buffer
    uint32_t    nr_ind; // Nr Industries in the buffer
    uint32_t    nr_cam; // Nr campains in the buffer
    uint32_t    nr_con; // Nr contacts in the buffer
    uint32_t    nr_cpy; // Nr contacts in the buffer
} dbc;

#endif

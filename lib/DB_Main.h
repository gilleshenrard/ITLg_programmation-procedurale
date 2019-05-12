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

#define SZ_CTY 100
#define SZ_JOB 200
#define SZ_IND 100
#define SZ_GRP 3000

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
    uint db_size;           // Taille de la database
    char db_name[SZ_NAME];  // Nom de la database
    uint sz_cty;            // Taille bloc country
    uint sz_job;            // Taille bloc job
    uint sz_ind;            // Taille bloc industry
    uint sz_grp;            // Taille bloc group
    uint sz_i_cty_name;     // Size of the index for countries (by name)
    long off_cty;           // Position bloc country
    long off_job;           // Position bloc job
    long off_ind;           // Position bloc industry
    long off_grp;           // Position bloc group
    long off_i_cty_name;    // Offset of the country index block (by name)
    long off_i_grp_fk;      // Offset of the group index block (by fk)
    long i_cty_name;        // Address of the root for the country index (by name)
    long i_grp_fk;          // Address of the root for the group index (by fk)
    char filler[44];        // filler to get the size to 128 bytes
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
    char nm_job[SZ_NAME_SM];
} cjob;

/***************************************************************************************
* Table Industry : dummy
****************************************************************************************/
typedef struct Industry
{
    char tp_rec[SZ_TYPE];   // Type de record IND
    int  id_ind;            // Cle primaire
    char nm_ind[SZ_NAME_SM];
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
    int id_cty;             // Foreign Key
    char filler[16];        // Filler to bring the group record size to 64 bytes
} cgrp;

/***************************************************************************************
* Structure base de donnees
****************************************************************************************/
typedef struct	db_country
{
    FILE*   fp;     // File pointer to the database
    hder    hdr;    // Header
    ccty*   cty;    // Buffer Country
    cgrp*   grp;    // Buffer Group
    int     nr_cty; // Nr elements dans buffer
    int     nr_grp; // Nr Groups in the buffer
} dbc;

#endif

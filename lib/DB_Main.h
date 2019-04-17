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
    uint off_cty;           // Position bloc country
    uint off_job;           // Position bloc job
    uint off_ind;           // Position bloc industry
    uint off_grp;           // Position bloc group
    uint off_i_cty_name;    // Offset of the country index block (by name)
    uint i_cty_name;        // Address of the root for the country index (by name)
    char filler[52];        // filler to get the size to 128 bytes
} hder;

/***************************************************************************************
* Table Country
****************************************************************************************/
typedef struct Country
{
    char tp_rec[SZ_TYPE];   // Type de record CTY
    int  id_cty;            // Cle primaire
    char nm_zon[20];        // Nom de la zone geographique
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
    char nm_job[20];
} cjob;

/***************************************************************************************
* Table Industry : dummy
****************************************************************************************/
typedef struct Industry
{
    char tp_rec[SZ_TYPE];   // Type de record IND
    int  id_ind;            // Cle primaire
    char nm_ind[20];
} cind;

/***************************************************************************************
* Table Group : dummy
****************************************************************************************/
typedef struct Group
{
    char tp_rec[SZ_TYPE];   // Type de record GRP
    int  id_grp;            // Cle primaire
    char nm_grp[20];
} cgrp;

/***************************************************************************************
* Structure base de donnees
****************************************************************************************/
typedef struct	db_country
{
    FILE* fp;           // File pointer to the database
    hder hdr;           // Header
    ccty cty[SZ_CTY];   // Buffer Country
    int nr_cty;         // Nr elements dans buffer
} dbc;

#endif

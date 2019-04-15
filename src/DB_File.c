/****************************************************************************************
* Dossier 2 : Database "Analyse de donnees clients"
*
* Exemple de creation de la database : fichiers non geres globalement
*
* Programmation procedurale 2019 - E.Bosly - Version 0
****************************************************************************************/
#include "../lib/DB_File.h"

/****************************************************************************************
* Creation de la database sur base des constantes SZ_*
****************************************************************************************/
void Create_DB(dbc *db)
{
    int i;
    ccty cty;
    cjob job;
    cind ind;
    cgrp grp;
    FILE *fp_db, *fp_lg;

    memset(&db->hdr, 0, sizeof(hder));
    memset(&cty, 0, sizeof(ccty));

    fp_db = fopen(DB_file, "wb");
    fp_lg = fopen(log_file, "a");

    // Creation du header ----------------------------

    strcpy(db->hdr.db_name, "DB_Comp");

    db->hdr.sz_cty = SZ_CTY;
    db->hdr.sz_job = SZ_JOB;
    db->hdr.sz_ind = SZ_IND;
    db->hdr.sz_grp = SZ_GRP;
    db->hdr.sz_i_cty_name = SZ_CTY;

    db->hdr.off_cty = sizeof(hder);
    db->hdr.off_job = db->hdr.off_cty + SZ_CTY * sizeof(ccty);
    db->hdr.off_ind = db->hdr.off_job + SZ_JOB * sizeof(cjob);
    db->hdr.off_grp = db->hdr.off_ind + SZ_IND * sizeof(cind);
    db->hdr.off_i_cty_name = db->hdr.off_grp + SZ_GRP * sizeof(cgrp);
    db->hdr.db_size = db->hdr.off_i_cty_name + SZ_CTY * sizeof(i_ccty_name);

    db->hdr.i_cty_name = 0;

    fwrite(&db->hdr, 1, sizeof(db->hdr), fp_db);

    // Creation de la table country ----------------------------

    memset(&cty, 0, sizeof(ccty));
    strcpy(cty.tp_rec, "CTY");

    for (i=0; i<SZ_CTY; i++)
        fwrite(&cty, 1, sizeof(ccty), fp_db);

    // Creation de la table job ----------------------------

    memset(&job, 0, sizeof(cjob));
    strcpy(job.tp_rec, "JOB");

    for (i=0; i<SZ_JOB; i++)
        fwrite(&job, 1, sizeof(cjob), fp_db);

    // Creation de la table industry ----------------------------

    memset(&ind, 0, sizeof(cind));
    strcpy(ind.tp_rec, "IND");

    for (i=0; i<SZ_IND; i++)
        fwrite(&ind, 1, sizeof(cind), fp_db);

    // Creation de la table group ----------------------------

    memset(&grp, 0, sizeof(cgrp));
    strcpy(grp.tp_rec, "GRP");

    for (i=0; i<SZ_GRP; i++)
        fwrite(&grp, 1, sizeof(cgrp), fp_db);

    fprintf(fp_lg, "Database %s created\n", db->hdr.db_name);

    fclose(fp_db);
    fclose(fp_lg);

    printf("Databse %s created \n", db->hdr.db_name);

    return;
}
/************************************************************************************/
/*  I : database in which create the index                                          */
/*  P : Creates the slots for the requested index at the end of the database        */
/*  O :  0 if OK                                                                    */
/*      -1 otherwise                                                                */
/************************************************************************************/
int create_index_unbuffered(dbc* db){
    i_ccty_name index_cty_name;
    FILE *fp_db=NULL, *fp_lg=NULL;

    //open the files and position the pointers at the end
    fp_db = fopen(DB_file, "a+b");
    fp_lg = fopen(log_file, "a");

    if(!fp_db || !fp_lg)
        return -1;

    //clean the buffer memory space
    memset(&index_cty_name, 0, sizeof(i_ccty_name));
    strcpy(index_cty_name.tp_rec, "I_CTY");

    //write all the slots sequentially
    for (int i=0; i<SZ_CTY; i++)
        fwrite(&index_cty_name, 1, sizeof(i_ccty_name), fp_db);

    //add a log entry after the creation
    fprintf(fp_lg, "Index %s created\n", "I_CTY");

    fclose(fp_db);
    fclose(fp_lg);

    return 0;
}

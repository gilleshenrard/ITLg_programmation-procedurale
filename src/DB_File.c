/****************************************************************************************
* Dossier 2 : Database "Analyse de donnees clients"
*
* Exemple de creation de la database : fichiers non geres globalement
*
* Programmation procedurale 2019 - E.Bosly - Version 0
****************************************************************************************/
#include "DB_File.h"

/****************************************************************************************
* Creation de la database sur base des constantes SZ_*
****************************************************************************************/
int Create_DB(dbc *db, char filename[])
{
    int i;
    ccty cty = {0};
    cjob job = {0};
    cind ind = {0};
    cgrp grp = {0};
    ccam cam = {0};
    ccon con = {0};
    ccpy cpy = {0};
    FILE *fp_lg = NULL;

    memset(db, 0, sizeof(dbc));
    db->fp = fopen(DB_file, "wb");
    if(!db->fp)
        return -1;

    fp_lg = fopen(log_file, "a");
    if(!fp_lg){
        fclose(db->fp);
        return -1;
    }

    // Creation du header ----------------------------
    strcpy(db->hdr.db_name, filename);

    db->hdr.sz_cty = SZ_CTY;
    db->hdr.sz_job = SZ_JOB;
    db->hdr.sz_ind = SZ_IND;
    db->hdr.sz_grp = SZ_GRP;
    db->hdr.sz_cam = SZ_CAM;
    db->hdr.sz_con = SZ_CON;
    db->hdr.sz_cpy = SZ_CPY;

    db->hdr.off_cty = sizeof(hder);
    db->hdr.off_job = db->hdr.off_cty + SZ_CTY * sizeof(ccty);
    db->hdr.off_ind = db->hdr.off_job + SZ_JOB * sizeof(cjob);
    db->hdr.off_grp = db->hdr.off_ind + SZ_IND * sizeof(cind);
    db->hdr.off_cam = db->hdr.off_grp + SZ_GRP * sizeof(cgrp);
    db->hdr.off_con = db->hdr.off_cam + SZ_CAM * sizeof(ccam);
    db->hdr.off_cpy = db->hdr.off_con + SZ_CON * sizeof(ccon);
    db->hdr.db_size = db->hdr.off_cpy + SZ_CPY * sizeof(ccpy);

    fwrite(&db->hdr, 1, sizeof(db->hdr), db->fp);

    // Creation de la table country ----------------------------
    strcpy(cty.tp_rec, "CTY");
    for (i=0; i<SZ_CTY; i++)
        fwrite(&cty, 1, sizeof(ccty), db->fp);

    // Creation de la table job ----------------------------
    strcpy(job.tp_rec, "JOB");
    for (i=0; i<SZ_JOB; i++)
        fwrite(&job, 1, sizeof(cjob), db->fp);

    // Creation de la table industry ----------------------------
    strcpy(ind.tp_rec, "IND");
    for (i=0; i<SZ_IND; i++)
        fwrite(&ind, 1, sizeof(cind), db->fp);

    // Creation de la table group ----------------------------
    strcpy(grp.tp_rec, "GRP");
    for (i=0; i<SZ_GRP; i++)
        fwrite(&grp, 1, sizeof(cgrp), db->fp);

    // Creation de la table campain ----------------------------
    strcpy(cam.tp_rec, "CAM");
    for (i=0; i<SZ_CAM; i++)
        fwrite(&cam, 1, sizeof(ccam), db->fp);

    // Creation de la table contact ----------------------------
    strcpy(con.tp_rec, "CON");
    for (i=0; i<SZ_CON; i++)
        fwrite(&con, 1, sizeof(ccon), db->fp);

    // Creation de la table company ----------------------------
    strcpy(cpy.tp_rec, "CPY");
    for (i=0; i<SZ_CPY; i++)
        fwrite(&cpy, 1, sizeof(ccpy), db->fp);

    fprintf(fp_lg, "Database %s created\n", db->hdr.db_name);

    fclose(db->fp);
    fclose(fp_lg);

    printf("Databse %s created \n", db->hdr.db_name);

    return 0;
}

/************************************************************************************/
/*  I : database in which create the index                                          */
/*      metadata of the data structure to be indexed                                */
/*      number of records present in the table                                      */
/*      info about the index block (offset, root, size of an element)               */
/*      info about the table block (offset, root, size of an element)               */
/*  P : Creates the slots for the requested index at the end of the database,       */
/*          then fills them and chains them as a binary tree (file algo approach)   */
/*  O :  0 if OK                                                                    */
/*      -1 otherwise                                                                */
/************************************************************************************/
long create_index_file(dbc* db, meta_t* meta, uint32_t nb, t_datablock* i_block, t_datablock* t_block){
    void *t_buffer=NULL, *i_buffer=NULL;
    uint32_t i=0, tmp=0;
    long root=0;

    //open the files and position the pointers at the end
    db->fp = fopen(DB_file, "r+b");

    if(!db->fp)
        return -1;

    //allocate the memory for the index array + create a data table single element buffer
    meta->structure = calloc(nb, meta->elementsize);
    meta->nbelements = nb;
    t_buffer = calloc(1, t_block->elem_size);

    //read the proper table sequentially and fill the buffer with it
    fseek(db->fp, *t_block->block_off, SEEK_SET);
    for(i=0 ; i < nb ; i++){
        //get the current offset in the DB, read the current element
        //  and set its slot field
        tmp = ftell(db->fp);
        fread(t_buffer, t_block->elem_size, 1, db->fp);

        //copy the relevant info from the table elem in the index one
        //  + inform it about the element slot
        i_buffer = get_arrayelem(meta, i);
        (*i_block->doCopy)(i_buffer, t_buffer);
        (*i_block->setSlot)(i_buffer, &tmp);
    }

    //sort the index array
   quickSortArray(meta, 0, nb-1);

    //save the index offset in the header
    fseek(db->fp, 0, SEEK_END);
    *i_block->block_off = ftell(db->fp);

    //sequentially write the buffer in memory (without tree chaining)
    for(i=0 ; i < nb ; i++){
        fwrite(get_arrayelem(meta, i), meta->elementsize, 1, db->fp);
    }

    //create the binary tree chaining
    root = index_tree(db->fp, *i_block->block_off, nb, meta);

    //write the new header values to disk
    db->hdr.db_size += meta->elementsize*nb;
    *i_block->root_off = root;
    fseek(db->fp, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, db->fp);

    free(t_buffer);
    free(meta->structure);
    fclose(db->fp);

    return 0;
}

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
int Create_DB(dbc *db, char filename[])
{
    int i;
    ccty cty = {0};
    cjob job = {0};
    cind ind = {0};
    cgrp grp = {0};
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

    db->hdr.off_cty = sizeof(hder);
    db->hdr.off_job = db->hdr.off_cty + SZ_CTY * sizeof(ccty);
    db->hdr.off_ind = db->hdr.off_job + SZ_JOB * sizeof(cjob);
    db->hdr.off_grp = db->hdr.off_ind + SZ_IND * sizeof(cind);
    db->hdr.db_size = db->hdr.off_grp + SZ_GRP * sizeof(cgrp);

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
long create_index_file(dbc* db, t_algo_meta* meta, int nb, t_datablock* i_block, t_datablock* t_block){
    void *i_iterator=NULL, *buffer=NULL;
    int i=0;
    long root=0, tmp=0;

    //open the files and position the pointers at the end
    db->fp = fopen(DB_file, "r+b");

    if(!db->fp)
        return -1;

    //allocate the memory for the full size and single element buffer and set an iterator pointer to it
    meta->structure = calloc(nb, meta->elementsize);
    i_iterator = meta->structure;
    buffer = calloc(1, t_block->elem_size);

    //read the proper table sequentially and fill the buffer with it
    fseek(db->fp, *t_block->block_off, SEEK_SET);
    for(i=0 ; i < nb ; i++){
        //get the offset in the actual table and save it in the corresponding index element
        tmp = ftell(db->fp);
        (*meta->setSlot)(i_iterator, &tmp);

        //read the element and copy the selected field in the index element
        fread(buffer, t_block->elem_size, 1, db->fp);
        (*meta->doCopy)(i_iterator, buffer);

        //reset the buffer and increment the iterator
        memset(buffer, 0, t_block->elem_size);
        i_iterator += meta->elementsize;
    }

    //sort the buffer
    quickSort(meta, 0, nb-1);

    //save the index offset in the header
    fseek(db->fp, 0, SEEK_END);
    *i_block->block_off = ftell(db->fp);

    //sequentially write the buffer in memory (without tree chaining)
    i_iterator = meta->structure;
    for(i=0 ; i < nb ; i++){
        fwrite(i_iterator, meta->elementsize, 1, db->fp);
        i_iterator += meta->elementsize;
    }

    //create the binary tree chaining
    root = index_tree(db->fp, *i_block->block_off, nb, meta);

    //write the new header values to disk
    db->hdr.db_size += meta->elementsize*nb;
    *i_block->root_off = root;
    fseek(db->fp, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, db->fp);

    free(buffer);
    free(meta->structure);
    fclose(db->fp);

    return 0;
}

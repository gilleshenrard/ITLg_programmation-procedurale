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
    db->hdr.db_size = db->hdr.off_grp + SZ_GRP * sizeof(cgrp);

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
    quickSort(meta, 0, nb);

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

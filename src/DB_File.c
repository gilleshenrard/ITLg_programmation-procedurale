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

/************************************************************/
/*  I : File pointer to the database                        */
/*      Offset of the first element of the data block       */
/*      Number of elements in the data block                */
/*      Metadata necessary to the algorithm                 */
/*  P : Assuming the block is filled with a known number of */
/*          sorted elements, the algo will chain its members*/
/*          as a binary tree (changes the indexes only)     */
/*  O : Offset of the current tree root                     */
/************************************************************/
/* WARNING : the index structure must finish with left and  */
/*              right long int types for indexes            */
/************************************************************/
long index_tree(FILE* fp, long offset_start, long nb, meta_t* meta){
    long old_offset=0, root=0, subtree=0;
    int nb_g=0, nb_d=0;

    //save the previous tree root offset
    old_offset = ftell(fp);

    //define the number of elements total (-1 in case of even number)
    //  and the number of elements on left and right
    nb_g = (nb-1)/2;
    nb_d = (nb-1) - nb_g;

    if(nb_g > 0){
        //set the file pointer to the "left child" field of the current root in the disk
        fseek(fp, offset_start + (nb_g*meta->elementsize) + (meta->elementsize - 2*sizeof(long)), SEEK_SET);

        //define the left child offset and write it in the proper field of the current root
        subtree = index_tree(fp, offset_start, nb_g, meta);
        fwrite(&subtree, sizeof(long), 1, fp);
    }
    if(nb_d > 0){
        //set the file pointer to the "right child" field of the current root in the disk
        fseek(fp, offset_start + (nb_g*meta->elementsize) + (meta->elementsize - sizeof(long)), SEEK_SET);

        //define the right child offset and write it in the proper field of the current root
        subtree = index_tree(fp, offset_start + (nb_g+1)*meta->elementsize, nb_d, meta);
        fwrite(&subtree, sizeof(long), 1, fp);
    }

    //get the offset of the current root
    fseek(fp, offset_start + nb_g*meta->elementsize, SEEK_SET);
    root = ftell(fp);

    //restore the previous tree root offset
    fseek(fp, old_offset, SEEK_SET);

    return root;
}

/************************************************************/
/*  I : File pointer to the database                        */
/*      Offset of the index tree root                       */
/*      Key to search in the index                          */
/*      Metadata of the array of index elements             */
/*      Metadata of the list of table elements              */
/*  P : Searches for the key in the index and add all       */
/*          matching elements in a list                     */
/*  O :  0 if OK                                            */
/*      -1 otherwise                                        */
/************************************************************/
/* WARNING : the index structure must finish with left and  */
/*              right long int types                        */
/************************************************************/
int searchall_index(FILE* fp, long offset_root, void* key, meta_t* index, meta_t* lis){
    void *index_buf=NULL, *table_buf=NULL;
    int comparison = 0;
    long offset = 0;

    //fill a buffer with the element of the current tree root
    index_buf = calloc(1, index->elementsize);
    fseek(fp, offset_root, SEEK_SET);
    fread(index_buf, 1, index->elementsize, fp);

    //compare it to the key received
    comparison = (*index->doCompare)(index_buf, key);
    if(!comparison){
        //get the offset of the corresponding element in the table
        fseek(fp, offset_root + (index->elementsize - 3*sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);

        //read the corresponding element and add it to the list
        fseek(fp, offset, SEEK_SET);
        table_buf = calloc(1, lis->elementsize);
        fread(table_buf, 1, lis->elementsize, fp);
        insertListSorted(lis, table_buf);
        free(table_buf);
    }

    //perform the search in the left subtree
    if(comparison >= 0){
        fseek(fp, offset_root + (index->elementsize - 2*sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);
        if(offset)
            searchall_index(fp, offset, key, index, lis);
    }

    //perform the search in the right subtree
    if(comparison <= 0){
        fseek(fp, offset_root + (index->elementsize - sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);
        if(offset)
            searchall_index(fp, offset, key, index, lis);
    }

    free(index_buf);
    return 0;
}

/************************************************************/
/*  I : File pointer to the database                        */
/*      Offset of the index tree root                       */
/*      Key to search in the index                          */
/*      Metadata necessary to the index algorithm           */
/*      element in which putting the search result          */
/*      Size of an element in the table                     */
/*  P : Searches for the first occurence of the key         */
/*          in the index                                    */
/*  O :  0 if OK                                            */
/*      -1 otherwise                                        */
/************************************************************/
/* WARNING : the index structure must finish with left and  */
/*              right long int types                        */
/************************************************************/
int searchone_index(FILE* fp, long offset_root, void* key, meta_t* index, void* elem, int elem_size){
    void *index_buf=NULL;
    int comparison = 0;
    long offset = 0;

    //fill a buffer with the element of the current tree root
    index_buf = calloc(1, index->elementsize);
    fseek(fp, offset_root, SEEK_SET);
    fread(index_buf, 1, index->elementsize, fp);

    //compare it to the key received
    comparison = (*index->doCompare)(index_buf, key);
    if(!comparison){
        //get the offset of the corresponding element in the table
        fseek(fp, offset_root + (index->elementsize - 3*sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);

        //read the corresponding element and add it to the list
        fseek(fp, offset, SEEK_SET);
        fread(elem, 1, elem_size, fp);

        free(index_buf);
        return 0;
    }

    if(comparison > 0){
        fseek(fp, offset_root + (index->elementsize - 2*sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);
        if(offset)
            searchone_index(fp, offset, key, index, elem, elem_size);
    }
    else{
        fseek(fp, offset_root + (index->elementsize - sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);
        if(offset)
            searchone_index(fp, offset, key, index, elem, elem_size);
    }

    free(index_buf);
    return 0;
}

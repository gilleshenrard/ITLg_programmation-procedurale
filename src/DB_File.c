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
int Create_DB(dbc *db, char filename[]){
    int i;
    ccty cty = {0};
    cjob job = {0};
    cind ind = {0};
    cgrp grp = {0};
    ccam cam = {0};
    ccon con = {0};
    ccpy cpy = {0};
    FILE *fp_lg = NULL;

    //open the DB and log files
    memset(db, 0, sizeof(dbc));
    db->fp = fopen(DB_file, "wb");
    if(!db->fp)
        return -1;

    fp_lg = fopen(log_file, "a");
    if(!fp_lg){
        fclose(db->fp);
        return -1;
    }

    //get the file pointer to after the header
    fseek(db->fp, sizeof(db->hdr), SEEK_SET);

    // Creation de la table country ----------------------------
    db->hdr.off_cty = ftell(db->fp);
    db->hdr.sz_cty = SZ_CTY;
    strcpy(cty.tp_rec, "CTY");
    for (i=0; i<SZ_CTY; i++)
        fwrite(&cty, 1, sizeof(ccty), db->fp);

    // Creation de la table job ----------------------------
    db->hdr.off_job = ftell(db->fp);
    db->hdr.sz_job = SZ_JOB;
    strcpy(job.tp_rec, "JOB");
    for (i=0; i<SZ_JOB; i++)
        fwrite(&job, 1, sizeof(cjob), db->fp);

    // Creation de la table industry ----------------------------
    db->hdr.off_ind = ftell(db->fp);
    db->hdr.sz_ind = SZ_IND;
    strcpy(ind.tp_rec, "IND");
    for (i=0; i<SZ_IND; i++)
        fwrite(&ind, 1, sizeof(cind), db->fp);

    // Creation de la table group ----------------------------
    db->hdr.off_grp = ftell(db->fp);
    db->hdr.sz_grp = SZ_GRP;
    strcpy(grp.tp_rec, "GRP");
    for (i=0; i<SZ_GRP; i++)
        fwrite(&grp, 1, sizeof(cgrp), db->fp);

    // Creation de la table campain ----------------------------
    db->hdr.off_cam = ftell(db->fp);
    db->hdr.sz_cam = SZ_CAM;
    strcpy(cam.tp_rec, "CAM");
    for (i=0; i<SZ_CAM; i++)
        fwrite(&cam, 1, sizeof(ccam), db->fp);

    // Creation de la table contact ----------------------------
    db->hdr.off_con = ftell(db->fp);
    db->hdr.sz_con = SZ_CON;
    strcpy(con.tp_rec, "CON");
    for (i=0; i<SZ_CON; i++)
        fwrite(&con, 1, sizeof(ccon), db->fp);

    // Creation de la table company ----------------------------
    db->hdr.off_cpy = ftell(db->fp);
    db->hdr.sz_cpy = SZ_CPY;
    strcpy(cpy.tp_rec, "CPY");
    for (i=0; i<SZ_CPY; i++)
        fwrite(&cpy, 1, sizeof(ccpy), db->fp);

    // write the header
    db->hdr.db_size = ftell(db->fp);
    fseek(db->fp, 0, SEEK_SET);
    fwrite(&db->hdr, 1, sizeof(db->hdr), db->fp);

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

    free(t_buffer);

    //sort the index array
   quickSortArray(meta, 0, nb-1);

    //save the index offset in the header
    fseek(db->fp, 0, SEEK_END);
    *i_block->block_off = ftell(db->fp);

    //sequentially write the buffer in memory (without tree chaining)
    for(i=0 ; i < nb ; i++){
        if(fwrite(get_arrayelem(meta, i), meta->elementsize, 1, db->fp) != 1)
            print_error("create_index_file() : error while writing element %d of the index\n", i);
    }

    //create the binary tree chaining
    root = index_tree(db->fp, *i_block->block_off, nb, meta);

    //write the new header values to disk
    db->hdr.db_size += meta->elementsize*nb;
    *i_block->root_off = root;
    fseek(db->fp, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, db->fp);

    empty_array(meta);
    fclose(db->fp);

    return 0;
}

/****************************************************************************************/
/*  I : Database from which import the records in memory                                */
/*      Metadata of the array in which load the table                                   */
/*      Offset of the data block to load                                                */
/*  P : Read the datablock of a DB table and load it in memory                          */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int Load_table(dbc *db, meta_t* dArray, long blockOffset){
    uint64_t i;
    void* buf = NULL;
	FILE *fp_lg;

	//open the DB and log files
    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    //make sure the array is empty beforehand
    if(dArray->structure)
        empty_array(dArray);

    //allocate memory for the new array
    dArray->structure = calloc(dArray->nbelements, dArray->elementsize);

    //get to the offset of the data block to load
    fseek(db->fp, blockOffset, SEEK_SET);

    //read every record and save it in the array
    buf = calloc(1, dArray->elementsize);
    for (i=0; i < dArray->nbelements; i++)
    {
        //read a record
        memset(buf, 0, dArray->elementsize);
        fread(buf, 1, dArray->elementsize, db->fp);

        //save it at the proper array slot
        set_arrayelem(dArray, i, buf);
    }
    free(buf);

    //confirm the table loading success
    fprintf(fp_lg, "table loaded into buffer : %lu\n", (unsigned long int)dArray->nbelements);
    printf("\nloaded into buffer : %lu\n\n", (unsigned long int)dArray->nbelements);

    //close the files
    fclose(db->fp);
    fclose(fp_lg);

    return 0;
}

/****************************************************************************************/
/*  I : Database from which export the block to a CSV file                              */
/*      Name of the export CSV file                                                     */
/*      Header to add to the CSV                                                        */
/*      Metadata of the DB block to export                                              */
/*      Amount of elements to export                                                    */
/*      Function allowing a table element to be formatted in a CSV line                 */
/*  P : Exports all the elements from a DB file block to a CSV file                     */
/*  O : -1 if error                                                                     */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int Export_CSV(dbc *db, char* filename, char* CSVheader, t_datablock* blockInfo, uint32_t nbElements, int (*doCSVFormat)(void* elem, char* finalLine)){
    uint32_t i;
	void* buf = NULL;
	char line[256] = "0";
	FILE *fpo, *fp_lg;

	//check if all info provided
	if(!db || !filename || !CSVheader || !blockInfo || !doCSVFormat){
        print_error("Export_CSV(): at least one information needed for the export is NULL");
        return -1;
	}

	//open the DB and log files
    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    //open the CSV export file
    printf("\nExporting to %s\n", filename);
    fpo = fopen(filename, "w");
    fprintf(fpo,CSVheader);

    //get to the beginning of the block to export
    fseek(db->fp, *blockInfo->block_off, SEEK_SET);

    //allocate memory for the buffer
    buf = calloc(1, blockInfo->elem_size);
    if(!buf){
        print_error("Export_CSV() : temporary element allocation error");
        fclose(fpo);
        fclose(db->fp);
        fclose(fp_lg);
        return -1;
    }

    //write all elements from the block in the CSV file
    for (i=0; i<nbElements; i++)
    {
        memset(buf, 0, blockInfo->elem_size);
        fread(buf, 1, blockInfo->elem_size, db->fp);

        (*doCSVFormat)(buf, line);
        fprintf(fpo,line);
    }

    //write log and inform user
    fprintf(fp_lg, "Successfully exported : %u lines\n", nbElements);
    printf("\nSuccessfully exported : %u\n\n lines", nbElements);

    free(buf);
    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpo);

    return 0;
}

/****************************************************************************************/
/*  I : Database to which import the CSV file to a data block                           */
/*      Name of the import CSV file                                                     */
/*      Offset of the block in the DB                                                   */
/*      Size of an record                                                               */
/*      Function allowing a CSV line to be transformed to a record                      */
/*  P : Imports all the lines from a CSV file to a DB data block                        */
/*  O : -1 if error                                                                     */
/*      Amount of records imported otherwise                                            */
/****************************************************************************************/
uint32_t Import_CSV(dbc *db, char* CSVfilename, uint32_t blockOffset, uint32_t elementSize, int (*doDeserialise)(char* line, void* record))
{
    uint32_t i=0;
    char line[BUF_LEN];
    char *ptr1;
	void* tmp = NULL;
	FILE *fpi, *fp_lg;

	//open the DB and log files
    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

    //open the import file
	fpi = fopen(CSVfilename, "r");

    //if error while opening files
	if (!fpi || !db->fp || !fp_lg) {
        print_error("Import_CSV() : Error while opening the requested files");
        return -1;
    }

    //read the first 200 characters
    fgets(line, BUF_LEN, fpi);

    //place the DB file pointer at the beginning of the data block to which the CSV
    //  will be imported
    fseek(db->fp, blockOffset, SEEK_SET);

    printf("Importing %s\n", CSVfilename);

    //read the whole CSV file, 200 characters at a time
    tmp = calloc(1, elementSize);
    while (fgets(line, BUF_LEN, fpi) != NULL)
    {
        //isolate the CSV line (until '\n') and deserialise it
        ptr1 = strtok(line,"\n");
        (*doDeserialise)(ptr1, tmp);

        //write the final record
        fwrite(tmp, 1, elementSize, db->fp);

        i++;
    }
    free(tmp);

    fprintf(fp_lg, "Imported : %lu\n", (unsigned long int)i);
    printf("Imported : %lu\n\n", (unsigned long int)i);

    //close all files
    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

	return i;
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

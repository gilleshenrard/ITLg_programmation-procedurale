#include "files_algo.h"

/************************************************************/
/*  I : File pointer to the database                        */
/*      Offset of the first element of the data block       */
/*      Number of elements in the data block                */
/*      Metadata necessary to the algorithm                 */
/*  P : Assuming the block is filled with a known number of */
/*          elements, the algo will chain its members as a  */
/*          binary tree (changes the pointers only)         */
/*  O : Offset of the current tree root                     */
/************************************************************/
/* WARNING : the index structure must finish with left and  */
/*              right long int types                        */
/************************************************************/
long index_tree(FILE* fp, long offset_start, long nb, t_algo_meta* meta){
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
/*      Metadata necessary to the index algorithm           */
/*      Metadata necessary to the list algorithm            */
/*      Size of an element in the table                     */
/*  P : Searches for the key in the index and add all       */
/*          matching elements in a list                     */
/*  O :  0 if OK                                            */
/*      -1 otherwise                                        */
/************************************************************/
/* WARNING : the index structure must finish with left and  */
/*              right long int types                        */
/************************************************************/
int searchall_index(FILE* fp, long offset_root, void* key, t_algo_meta* index, t_algo_meta* list, int elem_size){
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
        table_buf = calloc(1, elem_size);
        fread(table_buf, 1, elem_size, fp);
        insertListSorted(list, table_buf);
        free(table_buf);
    }

    //perform the search in the left subtree
    if(comparison >= 0){
        fseek(fp, offset_root + (index->elementsize - 2*sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);
        if(offset)
            searchall_index(fp, offset, key, index, list, elem_size);
    }

    //perform the search in the right subtree
    if(comparison <= 0){
        fseek(fp, offset_root + (index->elementsize - sizeof(long)), SEEK_SET);
        fread(&offset, 1, sizeof(long), fp);
        if(offset)
            searchall_index(fp, offset, key, index, list, elem_size);
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
int searchone_index(FILE* fp, long offset_root, void* key, t_algo_meta* index, void* elem, int elem_size){
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

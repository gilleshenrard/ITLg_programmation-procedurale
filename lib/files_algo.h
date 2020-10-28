#ifndef FILES_ALGO_H_INCLUDED
#define FILES_ALGO_H_INCLUDED
#include "algo.h"

//File binary trees
long index_tree(FILE* fp, long offset_start, long nb, t_algo_meta* meta);
int searchall_index(FILE* fp, long offset_root, void* key, t_algo_meta* index, t_algo_meta* list, int elem_size);
int searchone_index(FILE* fp, long offset_root, void* key, t_algo_meta* index, void* element, int elem_size);

#endif // FILES_ALGO_H_INCLUDED

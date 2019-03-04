# Programmation procedurale
## Dossier 2
---
### 1. Intro
This assignment aims to apply recursive structures and dynamic allocation by creating a pseudo DBMS

### 2. Set up
The assignment is programmed in C98, compiled with GNU GCC.

### 3. Current features (not developped by the teacher)
* Sorting algorithms :
```C
int bubbleSort(t_algo_meta*);
int bubbleSortList(t_algo_meta*);
int quickSortPartitioning(t_algo_meta*, int, int);
int quickSort(t_algo_meta*, int, int);
```
* Searching algorithms :
```C
int binarySearch(t_algo_meta*, void*);
int binarySearchFirst(t_algo_meta*, void*);
```
* Dynamic lists manipulation :
```C
int insertListTop(t_algo_meta*, void*);
int popListTop(t_algo_meta*);
int insertListSorted(t_algo_meta*,  void*);
int foreachList(t_algo_meta*, void*, int (*doAction)(void*, void*));
```
* Arrays manipulation :
```C
int foreachArray(t_algo_meta*, void*, int (*doAction)(void*, void*));
```

* AVL trees manipulation :
```C
void* insertAVL(t_algo_meta* meta, void* avl, void* toAdd);
void display_AVL_tree(t_algo_meta* meta, void* avl, char dir, char* (*toString)(void*));
void* rotate_AVL(t_algo_meta* meta, void* avl, e_rotation side);
int get_AVL_balance(t_algo_meta* meta, void* avl);
int foreachAVL(t_algo_meta* meta, void* avl, void* parameter, int (*doAction)(void*, void*));
void* search_AVL(t_algo_meta* meta, void* avl, void* key);
```

* Structures transformation (one format to another) :
```C
int listToArray(t_algo_meta* dList, t_algo_meta* dArray, e_listtoarray action);
int arrayToList(t_algo_meta* dArray, t_algo_meta* dList, e_listtoarray action);
int arrayToAVL(t_algo_meta* dArray, t_algo_meta* dAVL, e_listtoarray action);
```

* Unit tests for all existing procedures, in [DB_Main.c](https://github.com/gilleshenrard/ITLg_programmation-procedurale/blob/master/DB_Main.c)

### 4. Currently implemented in the final assignment
None

### 5. To Do
* Test AVL files instead of dynamically allocated
* Implement menus manipulation
* Implement unit tests

### 6. Known issues
* Unit tests for AVL work fine when the executable is launched, but fail while debugging
		(most probably because at some point, the children leaves address is 0xFFFEFFFE)

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

* Unit tests for all existing procedures, in [DB_Main.c](https://github.com/gilleshenrard/ITLg_programmation-procedurale/blob/master/DB_Main.c)

### 4. Currently implemented in the final assignment
None

### 5. To Do
* Implement binary trees manipulation
* Implement menus manipulation
* Implement unit tests

### 6. Known issues
None

#include "../lib/inout.h"

int appendTuple(FILE* file, t_tuple* tuple){
    int ret;

    fseek(file, 0, SEEK_END);
    ret = fwrite(tuple, sizeof(t_tuple), 1, file);
    if(ret!=1)
        return -1;

    return 0;
}

#ifndef LIST_STRUCT_H
#define LIST_STRUCT_H

#include <stdlib.h>

const int MAX_LIST_SIZE = 100000;
const int POIZON_IND    = -1;
const int POIZON_VAL    = 0XBADBAD;

const int CANARY_LEFT   = 0XABCD;
const int CANARY_RIGHT  = 0XDCBA;

struct Node_t
{
    int value = POIZON_VAL;
    int next  = POIZON_IND;
    int prev  = POIZON_IND;
};

struct list_t
{
    Node_t* data     = {};         
    int     free     = -1;     
    size_t  capacity =  0;
    int     size     = -1;
};

#endif
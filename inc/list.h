#ifndef LIST_H
#define LIST_H

#include "list_struct.h"
#include "../errors/errors.h"

list_t*  List_init        ( size_t  size );
error_t  List_dest        ( list_t* list );
error_t  List_realloc     ( list_t* list );
int      Insert_after     ( list_t* list, int anchor, int value );
int      Insert_befor     ( list_t* list, int anchor, int value );
int      Delete_elem      ( list_t* list, int index );
error_t  List_verify      ( list_t* list, int index = 1 );

#endif
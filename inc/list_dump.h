#ifndef LIST_DUMP_H
#define LIST_DUMP_H

#include "list_struct.h"

#define List_dump(list) List_dump_(list, Var_info{#list, __FILE__, __FUNCTION__, __LINE__})

struct Var_info 
{
    const char* name;
    const char* file_name;   
    const char* func_name;
    int line;
};

void List_dump_      ( list_t* list, Var_info    varinfo   ); 
void Text_listing    ( list_t* list, FILE*       dump_file );
void Image_list_dump ( list_t* list, const char* file_name ); 

void Clear_dump_html();

#endif


#include <stdio.h>
#include "../errors/errors.h"
#include "../inc/list_dump.h"
#include "../inc/list.h"
#include "../inc/list_struct.h"
#include "../inc/folders.h"

int main()
{
    printf("Program start\n");

    MakeDir();

    Clear_dump_html();

    list_t* list = List_init(6);

    Insert_after(list, 0, 10);

    Insert_after(list, 1, 20);
    
    Insert_after(list, 2, 30);

    List_realloc(list);

    Insert_after(list, 3, 40);

    List_dest(list);

    printf("Program end\n");

    return 0;
}
#include "../errors/errors.h"
#include "../inc/list_struct.h"
#include "../inc/list.h"
#include "../inc/list_dump.h"

//============================================(list_data)====================================================================================================

list_t* List_init( size_t size )
{
    my_assert(size > 0, SIZE_ERR, NULL);

    list_t* list = (list_t*)calloc(1, sizeof(list_t));
    my_assert(list, CALLOC_ERR, NULL);

    list -> capacity = size;
    list -> size = 0;

    list -> data = (Node_t*)calloc(size + 2, sizeof(Node_t));
    my_assert(list -> data, CALLOC_ERR, NULL);

    list -> free = 1;

    list -> data[0].value = CANARY_LEFT;
    list -> data[0].prev = list -> data[0].next = 0;

    for (size_t i = 1; i <= size; ++i){ //почему ++ перед
        list -> data[i].value = POIZON_VAL;

        if (i != size)
            list -> data[i].next = i + 1;
        
        else   
            list -> data[i].next = POIZON_IND;
        
        list -> data[i].prev = POIZON_IND;
    }

    list -> data[size + 1].value = CANARY_RIGHT;
    list -> data[size + 1].next = list -> data[size + 1].prev = POIZON_IND;

    List_dump(list);

    return list;
}

error_t List_dest( list_t* list )
{
    my_assert(list, NULLPTR, NULLPTR);

    list -> capacity = POIZON_IND;
    list -> size     = POIZON_IND;
    list -> free     = POIZON_IND;
    
    if (list -> data)
        free(list -> data);
    
    list -> data = nullptr;

    free(list);
    list = nullptr;

    return NO_ERROR;
}

error_t List_realloc( list_t* list )
{
    my_assert(list, NULLPTR, NULLPTR);

    size_t new_size = list -> capacity * 2;

    Node_t* new_data = (Node_t*)realloc(list -> data, (new_size + 2)*sizeof(Node_t));
    my_assert(new_data, REALLOC_ERR, REALLOC_ERR);

    list -> data = new_data;

    for (size_t i = list -> size + 1; i <= new_size; ++i){
        list -> data[i].value = POIZON_VAL;

        if (i != new_size)
            list -> data[i].next = i + 1;
        
        else   
            list -> data[i].next = POIZON_IND;
        
        list -> data[i].prev = POIZON_IND;
    }

    list -> data[new_size + 1].value = CANARY_RIGHT;
    list -> data[new_size + 1].prev = list -> data[new_size + 1].next = -1;

    list -> free = list -> size + 1;

    list -> capacity *= 2;

    return NO_ERROR;
}

//============================================(list_fuctions)====================================================================================================

int Insert_after( list_t* list, int anchor, int value )
{
    error_t error_code = List_verify(list, anchor);

    my_assert(error_code == NO_ERROR || error_code == REALLOC_ERR, error_code, -1);

    List_dump(list);

    if (list -> free == POIZON_IND || error_code == REALLOC_ERR)
        error_code = List_realloc(list);

        if (error_code == REALLOC_ERR)
        return -1;

        int new_ind = list -> free;
    list -> free = list -> data[new_ind].next;

    list -> data[new_ind].value = value;

    list -> data[new_ind].next = list -> data[anchor].next;
    list -> data[anchor].next = new_ind;

    list -> data[list -> data[new_ind].next].prev = new_ind;
    list -> data[new_ind].prev = anchor;

    ++list -> size;

    List_dump(list);

    return new_ind;
}

int Insert_befor( list_t* list, int anchor, int value )
{
    error_t error_code = List_verify(list, anchor);
    my_assert(error_code == NO_ERROR || error_code == REALLOC_ERR, error_code, -1);

    List_dump(list);

    if (list -> free == POIZON_IND || error_code == REALLOC_ERR)
        error_code = List_realloc(list);

    if (error_code == REALLOC_ERR)
        return -1;

    int new_ind = list -> free;
    list -> free = list -> data[new_ind].next;

    list -> data[new_ind].value = value;

    list -> data[new_ind].prev = list -> data[anchor].prev;
    list -> data[anchor].prev = new_ind;

    list -> data[new_ind].next = anchor;
    list -> data[list -> data[new_ind].prev].next = new_ind;

    ++list -> size;

    List_dump(list);

    return new_ind;   
}

int Delete_elem( list_t* list, int index )
{
    error_t error_code = List_verify(list, index);
        
    my_assert(error_code == NO_ERROR || error_code == REALLOC_ERR, error_code, -1);
    
    list -> data[index].value = POIZON_VAL;

    my_assert(list -> data[index].prev != POIZON_IND, IND_ERR, IND_ERR);
    
    list -> data[list -> data[index].next].prev = list -> data[index].prev;
    list -> data[list -> data[index].prev].next = list -> data[index].next;

    list -> data[index].next = list -> free;
    list -> data[index].prev = POIZON_IND;

    list -> free = index;

    --list -> size;

    List_dump(list);

    return NO_ERROR;
}

//============================================(list_verify)====================================================================================================

error_t List_verify( list_t* list, int index )
{
    if (!list)
        return NULLPTR;
    
    if (!list -> data)
        return NULLPTR;

    if (list -> capacity <= 0)
        return CAPACITY_ERR;

    if (list -> size < 0 || list -> size > MAX_LIST_SIZE)
        return SIZE_ERR;

    if (list -> capacity < list -> size)
        return CAP_SIZE_ERR;
    
    if (list -> free <= 0 || list -> free > list -> capacity)
        return FREE_ERR;

    if (list -> data[0].value != CANARY_LEFT || list -> data[list -> capacity + 1].value != CANARY_RIGHT)
        return CANARY_ERR;

    int free_cnt = 0;
    for (int i = list -> free; i != -1; i = list -> data[i].next) {
        if (i < 0 || i > list -> capacity + 1)
            return FREE_ERR;
        
        ++free_cnt;
    }

    if (free_cnt + list -> size != list -> capacity){
        printf("free_cnt = %d, size = %d, capacity = %d\n", free_cnt, list -> size, list -> capacity);
        return CAPACITY_ERR;
    }

    if (index < 0 || index >= list -> capacity)
        return IND_ERR;
    

    if (list -> data[index].prev == -1)
        return REALLOC_ERR;

    return NO_ERROR;
}


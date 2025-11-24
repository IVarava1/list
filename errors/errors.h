#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#define RESET_COLOR  "\033[0m"
#define RED_COLOR    "\033[31m"
#define GREEN_COLOR  "\033[32m"
#define YELLOW_COLOR "\033[33m"
#define BLUE_COLOR   "\033[36m" 

#define line_info __FILE__, __FUNCTION__ , __LINE__

#define my_assert(condition, error_type, return_value) \
    if ((condition) == 0) { \
        print_error(error_type, line_info); \
        return return_value; \
    } \

#define ERROR(name, error_code) \
    name = error_code, \

enum error_t
{
    #include "errors_type.txt"
    NUMBER_OF_ERRORS
};

#undef ERROR

void print_error(error_t error_type, const char* file_name, const char* func_name, const int line_ind);

#endif
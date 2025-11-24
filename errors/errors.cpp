#include "errors.h"

void print_error(error_t error_type, const char* file_name, const char* func_name, const int line_ind)
{    
    #define ERROR(name, error_code) \
		case error_code: \
            printf(RED_COLOR "ERROR < " YELLOW_COLOR "%s" RED_COLOR " > in file -> %s, in func -> %s, on line -> %d !\n" RESET_COLOR, #name, file_name, func_name, line_ind); \
            break; \

    switch(error_type) { 
        #include "errors_type.txt" 
        default:
            printf(RED_COLOR "ERROR < " YELLOW_COLOR "%s" RED_COLOR " > in file -> %s, in function -> %s, on line -> %d !\n" RESET_COLOR, "UNKNOWN", file_name, func_name, line_ind); 
            break;
    }

    #undef ERROR
}

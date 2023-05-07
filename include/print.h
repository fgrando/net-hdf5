#ifndef PRINT_HEADER_
#define PRINT_HEADER_

#include <stdio.h>

#define PRINT_ERR(...) do {printf("ERROR [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }while(0)
#define DEBUG_PRINT(...) do {printf("[%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }while(0)

#endif
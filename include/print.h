#ifndef PRINT_HEADER_
#define PRINT_HEADER_

#include <stdio.h>

#define PRINT_INF(...) do {printf("INFO [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }while(0)
#define PRINT_ERR(...) do {printf("ERRO [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }while(0)
#define PRINT_DBG(...) do {printf("DEBG [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }while(0)

#endif
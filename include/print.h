#ifndef PRINT_HEADER_
#define PRINT_HEADER_

#include <stdio.h>

#define PRINT_INF(...) do {printf("INF [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }while(0)
#define PRINT_ERR(...) do {printf("ERR [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }while(0)
#define PRINT_DBG(...) do {printf("DEB [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }while(0)

#endif

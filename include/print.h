#ifndef PRINT_HEADER_
#define PRINT_HEADER_

#include <stdio.h>

extern int PRINT_LV;
// debug   0
// info    1
// warning 2
// error   3

#define PRINT_ERR(...) do {if (3 >= PRINT_LV) {printf("ERR [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }}while(0)
#define PRINT_WAR(...) do {if (2 >= PRINT_LV) {printf("WAR [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }}while(0)
#define PRINT_INF(...) do {if (1 >= PRINT_LV) {printf("INF [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }}while(0)
#define PRINT_DBG(...) do {if (0 >= PRINT_LV) {printf("DBG [%s:%d] ", __FUNCTION__, __LINE__); fprintf(stdout, __VA_ARGS__); printf("\n"); }}while(0)

#define PRINT_SET_LEVEL(level) do { PRINT_DBG("set log level to %d", level); PRINT_LV = level; }while(0)


#endif

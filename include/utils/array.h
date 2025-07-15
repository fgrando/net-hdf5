#ifndef F70909E1_347C_4935_B304_CCF09D44262A
#define F70909E1_347C_4935_B304_CCF09D44262A

#include <stdint.h>

typedef struct{
    float *buffer;
    int size;
} array_f32_t;

typedef struct{
    double *buffer;
    int size;
} array_d64_t;

typedef struct{
    uint32_t *buffer;
    int size;
} array_u32_t;

typedef struct{
    int32_t *buffer;
    int size;
} array_i32_t;

typedef struct{
    uint8_t *buffer;
    int rows;
    int cols;
} array_bytes_t;

void array_f32_init(array_f32_t *arr, int size);
void array_f32_free(array_f32_t *arr);

void array_d64_init(array_d64_t *arr, int size);
void array_d64_free(array_d64_t *arr);

void array_u32_init(array_u32_t *arr, int size);
void array_u32_free(array_u32_t *arr);

void array_i32_init(array_i32_t *arr, int size);
void array_i32_free(array_i32_t *arr);

void array_bytes_init(array_bytes_t *buf, int rows, int cols);
void array_bytes_free(array_bytes_t *buf);

#endif /* F70909E1_347C_4935_B304_CCF09D44262A */

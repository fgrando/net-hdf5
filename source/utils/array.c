#include "utils/array.h"
#include <stdlib.h>

void array_f32_init(array_f32_t *arr, int size) {
  arr->size = size;
  arr->buffer = (float *)calloc(size, sizeof(float));
}

void array_f32_free(array_f32_t *arr) {
  free(arr->buffer);
  arr->buffer = NULL;
  arr->size = 0;
}

void array_d64_init(array_d64_t *arr, int size) {
  arr->size = size;
  arr->buffer = (double *)calloc(size, sizeof(double));
}

void array_d64_free(array_d64_t *arr) {
  free(arr->buffer);
  arr->buffer = NULL;
  arr->size = 0;
}

void array_u32_init(array_u32_t *arr, int size) {
  arr->size = size;
  arr->buffer = (uint32_t *)calloc(size, sizeof(uint32_t));
}

void array_u32_free(array_u32_t *arr) {
  free(arr->buffer);
  arr->buffer = NULL;
  arr->size = 0;
}

void array_i32_init(array_i32_t *arr, int size) {
  arr->size = size;
  arr->buffer = (int32_t *)calloc(size, sizeof(int32_t));
}

void array_i32_free(array_i32_t *arr) {
  free(arr->buffer);
  arr->buffer = NULL;
  arr->size = 0;
}

void array_bytes_init(array_bytes_t *buf, int rows, int cols) {
  buf->rows = rows;
  buf->cols = cols;
  buf->buffer = (uint8_t *)calloc(rows * cols, sizeof(uint8_t));
}

void array_bytes_free(array_bytes_t *buf) {
  free(buf->buffer);
  buf->buffer = NULL;
  buf->rows = 0;
  buf->cols = 0;
}
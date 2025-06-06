#pragma once

#include <stddef.h>

#define ML_MAGIC 0x4d4c0001 // 'ML' in hex
#define ML_HEADER_SIZE sizeof(ml_header_t)

#define ML_FOR_EACH(ml, it, item) \
    for (ml_iterator_t it = {0, 0}; (item = ml_it(ml, &it)) != NULL; )

typedef struct {
    size_t magic;
} ml_header_t;

typedef struct {
    void *__buf;
    size_t size;
    size_t __capacity;
    size_t __item_size;
    size_t __index;
    void* (*__allocator)(size_t size);
    void (*__deallocator)(void *ptr);
} ml_t;

typedef struct {
    size_t __index;
    size_t index; // For compatibility with the original code
} ml_iterator_t;

ml_t *ml_create(size_t item_size, size_t initial_capacity, void* (*allocator)(size_t size),
                void (*deallocator)(void *ptr));
void ml_destroy(ml_t *ml);
void ml_add(ml_t *ml, const void *item);
void ml_clear(ml_t *ml);
void ml_compact(ml_t *ml);
void *ml_it(const ml_t *ml, ml_iterator_t *it);
// Fast iterator that skips removed items without checking for validity
// USE AFTER ml_compact() to ensure the index is valid
void *ml_it_fast(const ml_t *ml, ml_iterator_t *it);
void ml_remove(ml_t *ml, ml_iterator_t *it);
// Directly access to the item at the specified index without checking for removal
// USE AFTER ml_compact() to ensure the index is valid
void *ml_get_by_direct_access_unsafe(const ml_t *ml, size_t index);
// Get the item at the specified index, checking for removal and validity
void *ml_get_by_direct_access(const ml_t *ml, size_t index);
// Find an item in the maplist by comparing its content
void *ml_find(const ml_t *ml, const void *item, size_t item_size);

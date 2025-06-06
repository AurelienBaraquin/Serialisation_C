#include <utils/maplist.h>
#include <string.h>

#include <stdio.h>

#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)

#define INITIAL_CAPACITY 16
#define ELEM_SIZE(ml) (ml->__item_size + sizeof(ml_header_t))

static inline int ml_check_capacity(const ml_t *ml) {
    if (UNLIKELY(ml->__index >= ml->__capacity)) {
        return 0; // Capacity exceeded
    }
    return 1;
}

static inline ml_header_t* ml_header() {
    static ml_header_t header = { .magic = ML_MAGIC };
    return &header;
}

ml_t *ml_create(size_t __item_size, size_t initial_capacity, void* (*__allocator)(size_t size),
                void (*__deallocator)(void *ptr)) {
    if (UNLIKELY(__item_size == 0 || initial_capacity == 0)) {
        // Handle error: invalid parameters
        return NULL;
    }

    ml_t *ml = (ml_t *)__allocator(sizeof(ml_t));
    if (UNLIKELY(ml == NULL)) {
        // Handle error: memory allocation failed
        return NULL;
    }

    ml->__buf = (__allocator)(__item_size * initial_capacity + sizeof(ml_header_t) * initial_capacity);
    if (UNLIKELY(ml->__buf == NULL)) {
        __deallocator(ml);
        // Handle error: memory allocation failed
        return NULL;
    }

    ml->__index = 0;
    ml->__capacity = initial_capacity;
    ml->__item_size = __item_size;
    ml->__allocator = __allocator;
    ml->__deallocator = __deallocator;

    return ml;
}

void ml_destroy(ml_t *ml) {
    if (UNLIKELY(ml == NULL)) {
        // Handle error: invalid maplist
        return;
    }

    if (ml->__buf != NULL) {
        ml->__deallocator(ml->__buf);
    }
    
    ml->__deallocator(ml);
}

static void ml_grow(ml_t *ml) {
    size_t new_capacity = ml->__capacity * 2;
    void *new_buf = ml->__allocator(new_capacity * ELEM_SIZE(ml));
    if (UNLIKELY(new_buf == NULL)) {
        // Handle error: memory allocation failed
        return;
    }

    ml_compact(ml); // Compact the memory before growing
    memcpy(new_buf, ml->__buf, ml->__index * ELEM_SIZE(ml));
    ml->__deallocator(ml->__buf);
    ml->__buf = new_buf;
    ml->__capacity = new_capacity;
}

void ml_add(ml_t *ml, const void *item) {
    size_t offset = ELEM_SIZE(ml) * ml->__index;
    if (ml_check_capacity(ml) == 0) {
        ml_grow(ml);
        if (UNLIKELY(ml->__index >= ml->__capacity)) {
            // Handle error: still out of capacity after growing
            return;
        }
    }
    void *dest = (char *)ml->__buf + offset;
    memcpy(dest, ml_header(), sizeof(ml_header_t));
    memcpy(dest + sizeof(ml_header_t), item, ml->__item_size);
    ml->__index++;
    ml->size++;
}

// remove dont move the whole memory, it only removes the item at the specified index by putting the magic byte at 0, later we will have a function to compact the memory
void ml_remove(ml_t *ml, ml_iterator_t *it) {
    if (UNLIKELY(ml == NULL)) {
        // Handle error: invalid maplist or index
        return;
    }

    size_t offset = ELEM_SIZE(ml) * it->__index;
    ml_header_t *header = (ml_header_t *)((char *)ml->__buf + offset);
    header->magic = 0;
    ml->size--;
}

void ml_compact(ml_t *ml) {
    size_t new_size = 0;
    for (size_t i = 0; i < ml->__index; i++) {
        size_t offset = i * ELEM_SIZE(ml);
        ml_header_t *header = (ml_header_t *)((char *)ml->__buf + offset);
        if (header->magic != 0) {
            if (new_size != i) {
                void *dest = (char *)ml->__buf + new_size * ELEM_SIZE(ml);
                memcpy(dest, header, ELEM_SIZE(ml));
            }
            new_size++;
        }
    }

    ml->__index = new_size;
}

void ml_clear(ml_t *ml) {
    for (size_t i = 0; i < ml->__index; ++i) {
        size_t offset = i * ELEM_SIZE(ml);
        ml_header_t *header = (ml_header_t *)((char *)ml->__buf + offset);
        header->magic = 0;
    }

    ml->__index = 0;
}

void *ml_it(const ml_t *ml, ml_iterator_t *it) {
    if (it->__index >= ml->__index) {
        // Handle error: iterator out of bounds
        return NULL;
    }

    size_t offset;
    int found = 0;

    do {
        offset = ELEM_SIZE(ml) * it->__index;
        ml_header_t *header = (ml_header_t *)((char *)ml->__buf + offset);
        if (header->magic == 0) {
            // If the magic byte is 0, it means the item is removed
            it->__index++;
        } else {
            found = 1;
            break;
        }
    } while (LIKELY(it->__index < ml->__index));

    if (UNLIKELY(it->__index >= ml->__index)) {
        // Handle error: iterator out of bounds after skipping removed items
        return NULL;
    }

    if (LIKELY(it->__index < ml->__index)) {
        it->__index++;
        it->index = it->__index - 1; // Update the public index
    }

    return found ? (char *)ml->__buf + offset + sizeof(ml_header_t) : NULL;
}

void *ml_it_fast(const ml_t *ml, ml_iterator_t *it) {
    if (UNLIKELY(it->__index >= ml->__index)) {
        return NULL;
    }

    size_t offset = ELEM_SIZE(ml) * it->__index;
    it->index = it->__index;
    it->__index++;

    return (char *)ml->__buf + offset + sizeof(ml_header_t);
}

size_t __ml_get_index_safe(const ml_t *ml, size_t index) {
    if (UNLIKELY(index >= ml->__index)) {
        // Handle error: invalid maplist or index
        return -1;
    }

    size_t offset;
    int found = 0;

    do {
        offset = ELEM_SIZE(ml) * index;
        ml_header_t *header = (ml_header_t *)((char *)ml->__buf + offset);
        if (header->magic == 0) {
            // If the magic byte is 0, it means the item is removed
            index++;
        } else {
            found = 1;
            break;
        }
    } while (LIKELY(index < ml->__index));

    return found ? index : -1; // Return -1 if not found
}

void *ml_get_by_direct_access_unsafe(const ml_t *ml, size_t index) {
    if (UNLIKELY(index >= ml->__index)) {
        // Handle error: invalid maplist or index
        return NULL;
    }

    size_t offset = ELEM_SIZE(ml) * index;
    return (char *)ml->__buf + offset + sizeof(ml_header_t);
}

void *ml_get_by_direct_access(const ml_t *ml, size_t index) {
    size_t safe_index = __ml_get_index_safe(ml, index);
    if (UNLIKELY(safe_index == (size_t)-1)) {
        // Handle error: invalid maplist or index
        return NULL;
    }

    return ml_get_by_direct_access_unsafe(ml, safe_index);
}

void *ml_find(const ml_t *ml, const void *item, size_t item_size) {
    if (UNLIKELY(ml == NULL || item == NULL || item_size != ml->__item_size)) {
        // Handle error: invalid maplist or item
        return NULL;
    }

    ml_iterator_t it = {0, 0};
    void *current_item;

    while ((current_item = ml_it(ml, &it)) != NULL) {
        if (memcmp(current_item, item, item_size) == 0) {
            return current_item; // Found the item
        }
    }

    return NULL; // Item not found
}

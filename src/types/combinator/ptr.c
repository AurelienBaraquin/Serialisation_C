#include "ser.h"
#include "utils/maplist.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    long addr;
    void* value;
} PtrEntry;

typedef struct {
    PtrEntry* entries;
    size_t count;
    size_t capacity;
} PtrRegistry;

PtrRegistry registry = {NULL, 0, 0};

PtrEntry* registry_find(long addr) {
    for (size_t i = 0; i < registry.count; i++) {
        if (registry.entries[i].addr == addr)
            return &registry.entries[i];
    }
    return NULL;
}

void registry_add(long addr, void* value) {
    if (registry.count >= registry.capacity) {
        registry.capacity = registry.capacity ? registry.capacity * 2 : 16;
        registry.entries = realloc(registry.entries, sizeof(PtrEntry) * registry.capacity);
    }
    registry.entries[registry.count++] = (PtrEntry){.addr = addr, .value = value};
}

void registry_clear() {
    free(registry.entries);
    registry.entries = NULL;
    registry.count = 0;
    registry.capacity = 0;
}

static void serialize_ptr(void* ptr, SerStream* out, const ser_type_t* self) {
    void* pval = *(void**)ptr;

    long addr = (long)pval;
    out->write(out, &addr, sizeof(long));

    if (!pval) return;

    if (!registry_find(addr)) {
        registry_add(addr, pval);
        self->data.ptr.subtype->serialize(pval, out, self->data.ptr.subtype);
    }
}

static void deserialize_ptr(void* ptr, SerStream* in, const ser_type_t* self) {
    long addr;
    in->read(in, &addr, sizeof(long));

    if (addr == 0) {
        *(void**)ptr = NULL;
        return;
    }

    PtrEntry* entry = registry_find(addr);
    if (entry) {
        *(void**)ptr = entry->value;
        return;
    }

    void* value = malloc(self->data.ptr.subtype->size);
    self->data.ptr.subtype->deserialize(value, in, self->data.ptr.subtype);
    *(void**)ptr = value;
    registry_add(addr, value);
}

static void free_ptr(void* ptr, ser_type_t* self) {
}

ser_type_t* ser_ptr(ser_type_t* subtype) {
    ser_type_t* ser_ptr_type = malloc(sizeof(ser_type_t));
    if (!ser_ptr_type) return NULL;

    *ser_ptr_type = (ser_type_t){
        .name = "pointer",
        .size = sizeof(void*),
        .kind = SER_KIND_COMBINATOR,
        .serialize = serialize_ptr,
        .deserialize = deserialize_ptr,
        .free = free_ptr,
        .data.ptr = {subtype}
    };

    return ser_ptr_type;
}

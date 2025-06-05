#include "ser.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    ser_type_t* subtype;
    size_t count;
} ser_array_info_t;

static void serialize_array(void* ptr, SerStream* out, const ser_type_t* self) {
    char* data = (char*)ptr;

    for (size_t i = 0; i < self->meta.array.count; ++i) {
        void* elem_ptr = data + i * self->meta.array.subtype->size;
        self->meta.array.subtype->serialize(elem_ptr, out, self->meta.array.subtype);
    }
}

static void deserialize_array(void* ptr, SerStream* in, const ser_type_t* self) {
    char* data = (char*)ptr;

    for (size_t i = 0; i < self->meta.array.count; ++i) {
        void* elem_ptr = data + i * self->meta.array.subtype->size;
        self->meta.array.subtype->deserialize(elem_ptr, in, self->meta.array.subtype);
    }
}

static void free_array(void* ptr, ser_type_t* self) {
    ser_type_t* subtype = self->meta.array.subtype;
    size_t count = self->meta.array.count;
    size_t stride = subtype->size;

    for (size_t i = 0; i < count; i++) {
        void* elem = (char*)ptr + i * stride;
        ser_free(elem, subtype);
    }
}

ser_type_t* ser_array(ser_type_t* subtype, size_t count) {
    ser_type_t* t = malloc(sizeof(ser_type_t));
    if (!t) {
        return NULL;
    }

    *t = (ser_type_t){
        .name = "array",
        .size = subtype->size * count,
        .kind = SER_KIND_ARRAY,
        .serialize = serialize_array,
        .deserialize = deserialize_array,
        .free = free_array,
        .meta.array.subtype = subtype,
        .meta.array.count = count
    };

    return t;
}

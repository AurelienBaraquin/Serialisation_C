#include "ser.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    ser_type_t* subtype;
    size_t count;
} ser_array_info_t;

static void serialize_array(void* ptr, SerStream* out, const ser_type_t* self) {
    char* data = (char*)ptr;

    for (size_t i = 0; i < self->data.array.count; ++i) {
        void* elem_ptr = data + i * self->data.array.subtype->size;
        self->data.array.subtype->serialize(elem_ptr, out, self->data.array.subtype);
    }
}

static void deserialize_array(void** ptr, SerStream* in, const ser_type_t* self) {
    char* data = (char*)*ptr;

    for (size_t i = 0; i < self->data.array.count; ++i) {
        void* elem_ptr = data + i * self->data.array.subtype->size;
        self->data.array.subtype->deserialize(elem_ptr, in, self->data.array.subtype);
    }
}

static void free_array(ser_type_t* self) {
    ser_type_t* subtype = self->data.array.subtype;

    ser_free(subtype);
    free(self);
}

ser_type_t* ser_array(ser_type_t* subtype, size_t count) {
    ser_type_t* ser_array_type = malloc(sizeof(ser_type_t));
    if (!ser_array_type) return NULL;

    *ser_array_type = (ser_type_t){
        .name = "array",
        .size = subtype->size * count,
        .serialize = serialize_array,
        .deserialize = deserialize_array,
        .free = free_array,
        .data.array = {
            .subtype = subtype,
            .count = count
        }
    };

    return ser_array_type;
}


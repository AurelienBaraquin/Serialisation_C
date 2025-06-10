#include "ser.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    ser_type_t* subtype;
    size_t count;
} ser_dyn_array_info_t;

static void serialize_dyn_array(void* ptr, SerStream* out, const ser_type_t* self) {
    char* data = (char*)ptr;
    size_t count = *self->data.dyn_array.count;

    out->write(out, &count, sizeof(count));

    for (size_t i = 0; i < count; ++i) {
        void* elem_ptr = data + i * self->data.dyn_array.subtype->size;
        self->data.dyn_array.subtype->serialize(elem_ptr, out, self->data.dyn_array.subtype);
    }
}

static void deserialize_dyn_array(void* ptr, SerStream* in, const ser_type_t* self) {
    char* data = (char*)ptr;
    size_t count;

    in->read(in, &count, sizeof(count));
    *self->data.dyn_array.count = count;

    for (size_t i = 0; i < count; ++i) {
        void* elem_ptr = data + i * self->data.dyn_array.subtype->size;
        self->data.dyn_array.subtype->deserialize(elem_ptr, in, self->data.dyn_array.subtype);
    }
}

static void free_dyn_array(void* ptr, ser_type_t* self) {
    ser_type_t* subtype = self->data.dyn_array.subtype;
    size_t count = *self->data.dyn_array.count;
    size_t stride = subtype->size;

    for (size_t i = 0; i < count; i++) {
        void* elem = (char*)ptr + i * stride;
        ser_free(elem, subtype);
    }
}

ser_type_t* ser_dyn_array(ser_type_t* subtype, size_t *count) {
    if (!subtype || !count) return NULL;

    ser_type_t* ser_dyn_array_type = malloc(sizeof(ser_type_t));
    if (!ser_dyn_array_type) return NULL;

    *ser_dyn_array_type = (ser_type_t){
        .name = "dyn_array",
        .size = sizeof(void*),
        .serialize = serialize_dyn_array,
        .deserialize = deserialize_dyn_array,
        .free = free_dyn_array,
        .data.dyn_array = {
            .subtype = subtype,
            .count = count
        }
    };

    return ser_dyn_array_type;
}

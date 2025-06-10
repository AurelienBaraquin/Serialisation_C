#include "ser.h"
#include <stdlib.h>

static void serialize_pointer(void* ptr, SerStream* out, const ser_type_t* self) {
    bool is_null = (*(void**)ptr == NULL);
    out->write(out, &is_null, sizeof(is_null));
    if (is_null) return;
    self->data.ptr.subtype->serialize(*(void**)ptr, out, self->data.ptr.subtype);
}

static void deserialize_pointer(void* ptr, SerStream* in, const ser_type_t* self) {
    bool is_null;
    in->read(in, &is_null, sizeof(is_null));
    if (is_null) {
        *(void**)ptr = NULL;
        return;
    }

    void** p = (void**)ptr;
    *p = malloc(self->data.ptr.subtype->size);

    self->data.ptr.subtype->deserialize(*p, in, self->data.ptr.subtype);
}

static void free_pointer(void* ptr, ser_type_t* self) {
}

ser_type_t* ser_pointer(ser_type_t* subtype) {
    ser_type_t* ser_pointer_type = malloc(sizeof(ser_type_t));
    if (!ser_pointer_type) return NULL;

    *ser_pointer_type = (ser_type_t){
        .name = "pointer",
        .size = sizeof(void*),
        .serialize = serialize_pointer,
        .deserialize = deserialize_pointer,
        .free = free_pointer,
        .data.ptr = {
            .subtype = subtype
        }
    };

    return ser_pointer_type;
}
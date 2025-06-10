#include "ser_types.h"
#include <stdlib.h>

void serialize_ser_primitive(void* ptr, SerStream* out, const ser_type_t* self) {
    out->write(out, ptr, self->size);
}

void deserialize_ser_primitive(void** ptr, SerStream* in, const ser_type_t* self) {
    if (!*ptr)
        *ptr = malloc(self->size);
    in->read(in, *ptr, self->size);
}

void ser_free_primitive(void* ptr, ser_type_t* self) {
    (void)ptr; // Primitive types do not require freeing the data
    free(self);
}

ser_type_t* ser_primitive(size_t size) {
    ser_type_t* ser_primitive_type = malloc(sizeof(ser_type_t));
    if (!ser_primitive_type) return NULL;

    *ser_primitive_type = (ser_type_t){
        .name = "primitive",
        .size = size,
        .serialize = serialize_ser_primitive,
        .deserialize = deserialize_ser_primitive,
        .free = NULL
    };

    return ser_primitive_type;
}

#define MAKE_SER_PRIMITIVE(name, type) \
ser_type_t* ser_##name() { \
    static ser_type_t* ser_##name##_type = NULL; \
    if (!ser_##name##_type) { \
        ser_##name##_type = ser_primitive(sizeof(type)); \
        if (!ser_##name##_type) return NULL; \
    } \
    return ser_##name##_type; \
}

MAKE_SER_PRIMITIVE(int, int)
MAKE_SER_PRIMITIVE(float, float)
MAKE_SER_PRIMITIVE(double, double)
MAKE_SER_PRIMITIVE(bool, bool)
MAKE_SER_PRIMITIVE(char, char)

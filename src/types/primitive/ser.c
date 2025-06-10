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

void ser_free_primitive(ser_type_t* self) {
    free(self);
}

ser_type_t* ser_primitive(char *name, size_t size) {
    ser_type_t* ser_primitive_type = malloc(sizeof(ser_type_t));
    if (!ser_primitive_type) return NULL;

    *ser_primitive_type = (ser_type_t){
        .name = name,
        .size = size,
        .serialize = serialize_ser_primitive,
        .deserialize = deserialize_ser_primitive,
        .free = ser_free_primitive,
    };

    return ser_primitive_type;
}

ser_type_t* ser_int() {
    return ser_primitive("int", sizeof(int));
}
ser_type_t* ser_float() {
    return ser_primitive("float", sizeof(float));
}
ser_type_t* ser_double() {
    return ser_primitive("double", sizeof(double));
}
ser_type_t* ser_bool() {
    return ser_primitive("bool", sizeof(bool));
}
ser_type_t* ser_char() {
    return ser_primitive("char", sizeof(char));
}

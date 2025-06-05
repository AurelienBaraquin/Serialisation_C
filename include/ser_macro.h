#ifndef SER_MACRO_H
#define SER_MACRO_H

#include "ser.h"

#define SER_FIELD(field, type) \
    { #field, offsetof(__STRUCT_TYPE__, field), &type }

#define SER_DECLARE_DEFINE(type_name, ...) \
    FieldDesc type_name##_fields[] = { \
        __VA_ARGS__ \
    }; \
    ser_type_t ser_##type_name = { \
        .name = #type_name, \
        .size = sizeof(type_name), \
        .serialize = serialize_struct, \
        .deserialize = deserialize_struct, \
        .free = free_struct, \
        .meta.st.fields = type_name##_fields, \
        .meta.st.field_count = sizeof(type_name##_fields) / sizeof(FieldDesc) \
    };

#define SER_DEFINE(type_name, ...) \
    FieldDesc type_name##_fields[] = { \
        __VA_ARGS__ \
    }; \
    ser_##type_name = (ser_type_t){ \
        .name = #type_name, \
        .size = sizeof(type_name), \
        .serialize = serialize_struct, \
        .deserialize = deserialize_struct, \
        .free = free_struct, \
        .meta.st.fields = type_name##_fields, \
        .meta.st.field_count = sizeof(type_name##_fields) / sizeof(FieldDesc) \
    };

#define SER_DECLARE(type) \
    ser_type_t ser_##type;

#endif

#ifndef SER_TYPE_H
#define SER_TYPE_H

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "ser_stream.h"

struct ser_type_t;

typedef struct {
    const char* name;
    size_t offset;
    struct ser_type_t* type;
} FieldDesc;

typedef enum {
    SER_KIND_PRIMITIVE,
    SER_KIND_COMBINATOR,
} ser_kind_t;

typedef struct {
    union {
        struct {
            FieldDesc* fields;
            size_t field_count;
        } st;

        struct {
            struct ser_type_t* subtype;
            size_t count;
        } array;

        struct {
            struct ser_type_t* subtype;
        } opt;

        struct {
            struct ser_type_t* subtype;
        } ptr;
    };
} ser_data_t;

typedef struct ser_type_t {
    const char* name;
    size_t size;

    ser_kind_t kind;

    void (*serialize)(void* ptr, SerStream* out, const struct ser_type_t* self);
    void (*deserialize)(void* ptr, SerStream* in, const struct ser_type_t* self);
    void (*free)(void* ptr, struct ser_type_t* self);

    ser_data_t data;
} ser_type_t;

#endif

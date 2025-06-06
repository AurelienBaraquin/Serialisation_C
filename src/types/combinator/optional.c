#include "ser.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void serialize_optional(void* ptr, SerStream* out, const ser_type_t* self) {
    ser_type_t* subtype = self->data.opt.subtype;

    uint8_t has_value = ((void**)ptr)[0] != NULL;
    out->write(out, &has_value, sizeof(uint8_t));
    if (has_value)
        subtype->serialize(*(void**)ptr, out, subtype);
}

static void deserialize_optional(void* ptr, SerStream* in, const ser_type_t* self) {
    ser_type_t* subtype = self->data.opt.subtype;

    uint8_t has_value;
    in->read(in, &has_value, sizeof(uint8_t));

    void **p = (void**)ptr;

    if (has_value) {
        *p = malloc(subtype->size);
        subtype->deserialize(*p, in, subtype);
    } else {
        *p = NULL;
    }
}

static void free_optional(void* ptr, ser_type_t* self) {
    ser_type_t* subtype = self->data.opt.subtype;
    void* pointeur = *(void**)ptr;
    if (pointeur) {
        ser_free(pointeur, subtype);
        free(pointeur);
    }
}

ser_type_t* ser_optional(ser_type_t* subtype) {
    ser_type_t* ser_opt_type = malloc(sizeof(ser_type_t));
    if (!ser_opt_type) return NULL;

    *ser_opt_type = (ser_type_t){
        .name = "optional",
        .size = sizeof(void*),
        .kind = SER_KIND_COMBINATOR,
        .serialize = serialize_optional,
        .deserialize = deserialize_optional,
        .free = free_optional,
        .data.opt = {subtype}
    };

    return ser_opt_type;
}

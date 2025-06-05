#include "ser.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void serialize_optional(void* ptr, SerStream* out, const ser_type_t* self) {
    ser_type_t* subtype = self->meta.opt.subtype;
    void* pointeur = *(void**)ptr;

    uint8_t has_value = (pointeur != NULL);
    out->write(out, &has_value, sizeof(uint8_t));
    if (has_value)
        subtype->serialize(pointeur, out, subtype);
}

static void deserialize_optional(void* ptr, SerStream* in, const ser_type_t* self) {
    ser_type_t* subtype = self->meta.opt.subtype;

    uint8_t has_value = 0;
    in->read(in, &has_value, sizeof(uint8_t));

    if (has_value) {
        void* target = malloc(subtype->size);
        subtype->deserialize(target, in, subtype);
        *(void**)ptr = target;
    } else {
        *(void**)ptr = NULL;
    }
}

static void free_optional(void* ptr, ser_type_t* self) {
    ser_type_t* subtype = self->meta.opt.subtype;
    void* pointeur = *(void**)ptr;
    if (pointeur) {
        ser_free(pointeur, subtype);
        free(pointeur);
    }
}

ser_type_t* ser_optional(ser_type_t* subtype) {
    ser_type_t* t = malloc(sizeof(ser_type_t));
    t->name = "optional";
    t->kind = SER_KIND_DYNAMIC;
    t->size = sizeof(void*);
    t->serialize = serialize_optional;
    t->deserialize = deserialize_optional;
    t->free = free_optional;
    t->meta.opt.subtype = subtype;
    return t;
}

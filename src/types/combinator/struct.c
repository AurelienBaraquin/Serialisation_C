#include "ser.h"
#include <stdarg.h>
#include <stdlib.h>

#define SER_DEFAULT_ALIGNMENT 8

void serialize_struct(void* ptr, SerStream* out, const ser_type_t* self) {
    for (size_t i = 0; i < self->data.st.field_count; i++) {
        const FieldDesc* field = &self->data.st.fields[i];
        void* field_ptr = (char*)ptr + field->offset;
        field->type->serialize(field_ptr, out, field->type);
    }
}

void deserialize_struct(void** ptr, SerStream* in, const ser_type_t* self) {
    for (size_t i = 0; i < self->data.st.field_count; i++) {
        const FieldDesc* field = &self->data.st.fields[i];
        void* field_ptr = (char*)*ptr + field->offset;
        field->type->deserialize((void **)&field_ptr, in, field->type);
    }
}

void free_struct(void* ptr, ser_type_t* self) {
    for (size_t i = 0; i < self->data.st.field_count; i++) {
        FieldDesc* f = &self->data.st.fields[i];
        void* field_ptr = (char*)ptr + f->offset;
        ser_free(field_ptr, f->type);
    }
    free(self);
}

static size_t align_up(size_t value, size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

ser_type_t* ser_struct(size_t nb, ...) {
    ser_type_t* ser_struct_type = malloc(sizeof(ser_type_t));
    if (!ser_struct_type) return NULL;

    *ser_struct_type = (ser_type_t){
        .name = "struct",
        .size = 0,
        .serialize = serialize_struct,
        .deserialize = deserialize_struct,
        .free = free_struct,
        .data.st.fields = NULL,
        .data.st.field_count = nb
    };

    va_list args;
    va_start(args, nb);
    ser_struct_type->data.st.fields = malloc(nb * sizeof(FieldDesc));
    if (!ser_struct_type->data.st.fields) {
        free(ser_struct_type);
        va_end(args);
        return NULL;
    }

    size_t struct_end = 0;

    for (size_t i = 0; i < nb; ++i) {
        FieldDesc* field = &ser_struct_type->data.st.fields[i];
        field->name = va_arg(args, const char*);
        field->offset = va_arg(args, size_t);
        field->type = va_arg(args, ser_type_t*);

        size_t end = field->offset + field->type->size;
        if (end > struct_end)
            struct_end = end;
    }
    ser_struct_type->size = align_up(struct_end, SER_DEFAULT_ALIGNMENT);

    return ser_struct_type;
}

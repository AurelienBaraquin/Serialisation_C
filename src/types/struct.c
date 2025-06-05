#include "ser.h"

void serialize_struct(void* ptr, SerStream* out, const ser_type_t* self) {
    for (size_t i = 0; i < self->meta.st.field_count; i++) {
        const FieldDesc* field = &self->meta.st.fields[i];
        void* field_ptr = (char*)ptr + field->offset;
        field->type->serialize(field_ptr, out, field->type);
    }
}

void deserialize_struct(void* ptr, SerStream* in, const ser_type_t* self) {
    for (size_t i = 0; i < self->meta.st.field_count; i++) {
        const FieldDesc* field = &self->meta.st.fields[i];
        void* field_ptr = (char*)ptr + field->offset;
        field->type->deserialize(field_ptr, in, field->type);
    }
}

void free_struct(void* ptr, ser_type_t* self) {
    for (size_t i = 0; i < self->meta.st.field_count; i++) {
        FieldDesc* f = &self->meta.st.fields[i];
        void* field_ptr = (char*)ptr + f->offset;
        ser_free(field_ptr, f->type);
    }
}

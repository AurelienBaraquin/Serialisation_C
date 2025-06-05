#include "ser_primitive.h"

static void serialize_char(void* ptr, SerStream* out, const ser_type_t* self) {
    char value = *(char*)ptr;
    out->write(out, &value, sizeof(char));
}

static void deserialize_char(void* ptr, SerStream* in, const ser_type_t* self) {
    char value;
    in->read(in, &value, sizeof(char));
    *(char*)ptr = value;
}

ser_type_t ser_char = {
    .name = "char",
    .size = sizeof(char),
    .kind = SER_KIND_PRIMITIVE,
    .serialize = serialize_char,
    .deserialize = deserialize_char,
    .meta.st.fields = NULL,
    .meta.st.field_count = 0
};

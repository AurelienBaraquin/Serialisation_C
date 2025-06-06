#include "ser_types.h"

static void serialize_char(void* ptr, SerStream* out, const ser_type_t* self) {
    out->write(out, ptr, sizeof(char));
}

static void deserialize_char(void* ptr, SerStream* in, const ser_type_t* self) {
    in->read(in, ptr, sizeof(char));
}

ser_type_t* ser_char() {
    static ser_type_t ser_char_type = {
        .name = "char",
        .size = sizeof(char),
        .kind = SER_KIND_PRIMITIVE,
        .serialize = serialize_char,
        .deserialize = deserialize_char,
        .free = NULL
    };
    return &ser_char_type;
}

#include "ser_types.h"

static void serialize_bool(void* ptr, SerStream* out, const ser_type_t* self) {
    out->write(out, ptr, sizeof(int));
}

static void deserialize_bool(void* ptr, SerStream* in, const ser_type_t* self) {
    in->read(in, ptr, sizeof(int));
}

ser_type_t* ser_bool() {
    static ser_type_t ser_bool_type = {
        .name = "bool",
        .size = sizeof(int),
        .kind = SER_KIND_PRIMITIVE,
        .serialize = serialize_bool,
        .deserialize = deserialize_bool,
        .free = NULL
    };
    return &ser_bool_type;
}

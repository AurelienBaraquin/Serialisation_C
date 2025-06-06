#include "ser_types.h"

static void serialize_int(void* ptr, SerStream* out, const ser_type_t* self) {
    out->write(out, ptr, sizeof(int));
}

static void deserialize_int(void* ptr, SerStream* in, const ser_type_t* self) {
    in->read(in, ptr, sizeof(int));
}

ser_type_t* ser_int() {
    static ser_type_t ser_int_type = {
        .name = "int",
        .size = sizeof(int),
        .kind = SER_KIND_PRIMITIVE,
        .serialize = serialize_int,
        .deserialize = deserialize_int,
        .free = NULL
    };
    return &ser_int_type;
}

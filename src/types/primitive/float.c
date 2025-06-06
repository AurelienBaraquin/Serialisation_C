#include "ser_types.h"

static void serialize_float(void* ptr, SerStream* out, const ser_type_t* self) {
    out->write(out, ptr, sizeof(float));
}

static void deserialize_float(void* ptr, SerStream* in, const ser_type_t* self) {
    in->read(in, ptr, sizeof(float));
}

ser_type_t* ser_float() {
    static ser_type_t ser_float_type = {
        .name = "float",
        .size = sizeof(float),
        .kind = SER_KIND_PRIMITIVE,
        .serialize = serialize_float,
        .deserialize = deserialize_float,
        .free = NULL
    };
    return &ser_float_type;
}

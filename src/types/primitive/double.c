#include "ser_types.h"

static void serialize_double(void* ptr, SerStream* out, const ser_type_t* self) {
    out->write(out, ptr, sizeof(double));
}

static void deserialize_double(void* ptr, SerStream* in, const ser_type_t* self) {
    in->read(in, ptr, sizeof(double));
}

ser_type_t* ser_double() {
    static ser_type_t ser_double_type = {
        .name = "double",
        .size = sizeof(double),
        .kind = SER_KIND_PRIMITIVE,
        .serialize = serialize_double,
        .deserialize = deserialize_double,
        .free = NULL
    };
    return &ser_double_type;
}

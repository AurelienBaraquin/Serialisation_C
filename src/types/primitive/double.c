#include "ser_primitive.h"

static void serialize_double(void* ptr, SerStream* out, const ser_type_t* self) {
    double value = *(double*)ptr;
    out->write(out, &value, sizeof(double));
}

static void deserialize_double(void* ptr, SerStream* in, const ser_type_t* self) {
    double value;
    in->read(in, &value, sizeof(double));
    *(double*)ptr = value;
}

ser_type_t ser_double = {
    .name = "double",
    .size = sizeof(double),
    .kind = SER_KIND_PRIMITIVE,
    .serialize = serialize_double,
    .deserialize = deserialize_double,
    .meta.st.fields = NULL,
    .meta.st.field_count = 0
};

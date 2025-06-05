#include "ser_primitive.h"

static void serialize_float(void* ptr, SerStream* out, const ser_type_t* self) {
    float value = *(float*)ptr;
    out->write(out, &value, sizeof(float));
}

static void deserialize_float(void* ptr, SerStream* in, const ser_type_t* self) {
    float value;
    in->read(in, &value, sizeof(float));
    *(float*)ptr = value;
}

ser_type_t ser_float = {
    .name = "float",
    .size = sizeof(float),
    .kind = SER_KIND_PRIMITIVE,
    .serialize = serialize_float,
    .deserialize = deserialize_float,
    .meta.st.fields = NULL,
    .meta.st.field_count = 0
};

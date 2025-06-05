#include "ser_primitive.h"

static void serialize_bool(void* ptr, SerStream* out, const ser_type_t* self) {
    int value = *(int*)ptr;
    out->write(out, &value, sizeof(int));
}

static void deserialize_bool(void* ptr, SerStream* in, const ser_type_t* self) {
    int value;
    in->read(in, &value, sizeof(int));
    *(int*)ptr = value;
}

ser_type_t ser_bool = {
    .name = "bool",
    .size = sizeof(int),
    .kind = SER_KIND_PRIMITIVE,
    .serialize = serialize_bool,
    .deserialize = deserialize_bool,
    .meta.st.fields = NULL,
    .meta.st.field_count = 0
};

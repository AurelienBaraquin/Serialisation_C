#include "ser_primitive.h"

static void serialize_int(void* ptr, SerStream* out, const ser_type_t* self) {
    int value = *(int*)ptr;
    out->write(out, &value, sizeof(int));
}
static void deserialize_int(void* ptr, SerStream* in, const ser_type_t* self) {
    int value;
    in->read(in, &value, sizeof(int));
    *(int*)ptr = value;
}

ser_type_t ser_int = {
    .name = "int",
    .size = sizeof(int),
    .kind = SER_KIND_PRIMITIVE,
    .serialize = serialize_int,
    .deserialize = deserialize_int,
    .meta.st.fields = NULL,
    .meta.st.field_count = 0
};

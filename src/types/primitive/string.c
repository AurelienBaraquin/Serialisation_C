#include "ser_primitive.h"
#include <stdlib.h>
#include <string.h>

static void serialize_string(void* ptr, SerStream* out, const ser_type_t* self) {
    char* str = *(char**)ptr;
    size_t len = strlen(str);
    out->write(out, &len, sizeof(size_t));
    out->write(out, str, len);
}
static void deserialize_string(void* ptr, SerStream* in, const ser_type_t* self) {
    size_t len;
    in->read(in, &len, sizeof(size_t));
    char* str = malloc(len + 1);
    in->read(in, str, len);
    str[len] = '\0';
    *(char**)ptr = str;
}

static void free_string(void* ptr, ser_type_t* self) {
    char* str = *(char**)ptr;
    if (str) free(str);
}

ser_type_t ser_string = {
    .name = "string",
    .size = sizeof(char*),
    .kind = SER_KIND_PRIMITIVE,
    .serialize = serialize_string,
    .deserialize = deserialize_string,
    .free = free_string,
    .meta.st.fields = NULL,
    .meta.st.field_count = 0
};

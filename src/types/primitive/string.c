#include "ser_types.h"
#include <stdlib.h>
#include <string.h>

static void serialize_string(void* ptr, SerStream* out, const ser_type_t* self) {
    size_t len = strlen(ptr);
    out->write(out, &len, sizeof(size_t));
    out->write(out, ptr, len);
}

static void deserialize_string(void* ptr, SerStream* in, const ser_type_t* self) {
    size_t len;
    in->read(in, &len, sizeof(size_t));

    char** str_ptr = (char**)ptr;
    *str_ptr = malloc(len + 1);
    in->read(in, *str_ptr, len);
    (*str_ptr)[len] = '\0';
}

static void free_string(void* ptr, ser_type_t* self) {
    if (ptr) {
        free(ptr);
    }
    self->free = NULL;
}

ser_type_t* ser_string() {
    static ser_type_t ser_string_type = {
        .name = "string",
        .size = sizeof(char*),
        .kind = SER_KIND_PRIMITIVE,
        .serialize = serialize_string,
        .deserialize = deserialize_string,
        .free = free_string
    };
    return &ser_string_type;
}

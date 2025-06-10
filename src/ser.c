#include "ser.h"
#include <stdlib.h>
#include "ser_stream.h"

void serialize(void* obj, SerStream* out, ser_type_t* type) {
    type->serialize(obj, out, type);
}

void deserialize(void* obj, SerStream* in, ser_type_t* type) {
    type->deserialize(&obj, in, type);
}

uint8_t* ser_serialize_to_dynamic_buffer(void* object, ser_type_t* type, size_t* out_size) {
    SerStream counter;
    ser_stream_init_counter(&counter);
    serialize(object, &counter, type);
    size_t size = ser_stream_get_count(&counter);
    ser_stream_free(&counter);

    uint8_t* buffer = malloc(size);
    if (!buffer) return NULL;
    if (out_size) *out_size = size;

    SerStream buffer_stream;
    ser_stream_init_buffer(&buffer_stream, buffer, size);
    serialize(object, &buffer_stream, type);
    ser_stream_free(&buffer_stream);

    return buffer;
}

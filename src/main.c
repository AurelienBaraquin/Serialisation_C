#include "ser.h"
#include "ser_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    char str[64];
    char ptr;
} Test;

int main() {
    char buffer[128] = {0};
    ser_type_t* ser_point_type = ser_struct(2,
        "str", offsetof(Test, str), ser_array(ser_char(), 64),
        "ptr", offsetof(Test, ptr), ser_char()
    );

    Test p = {
        .str = "Hello, World!",
        .ptr = 'A'
    };
    SerStream out_stream;
    ser_stream_init_buffer(&out_stream, (uint8_t*)buffer, sizeof(buffer));
    serialize(&p, &out_stream, ser_point_type);
    ser_stream_free(&out_stream);

    Test p2;
    SerStream in_stream;
    ser_stream_init_buffer(&in_stream, (uint8_t*)buffer, sizeof(buffer));
    deserialize(&p2, &in_stream, ser_point_type);
    ser_stream_free(&in_stream);

    printf("Deserialized string: %s, pointer: %c\n", p2.str, p2.ptr);
    return 0;
}

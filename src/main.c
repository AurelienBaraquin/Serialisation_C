#include "ser.h"
#include "ser_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define ARRAY_LEN 10

typedef struct {
    char *str;
} Test;

int main() {
    char buffer[64] = {0};
    ser_type_t* ser_point_type = ser_struct(1,
        "str", offsetof(Test, str), ser_ptr(ser_char())
    );

    Test p = {
        .str = "Hello, World!"
    };
    SerStream out_stream;
    ser_stream_init_buffer(&out_stream, (uint8_t*)buffer, sizeof(buffer));
    serialize(&p, &out_stream, ser_point_type);
    ser_stream_free(&out_stream);

    // print the raw buffer
    printf("Serialized data: ");
    for (size_t i = 0; i < sizeof(buffer); i++) {
        printf("%c ", buffer[i]);
    }
    printf("\n");

    Test p2;
    SerStream in_stream;
    ser_stream_init_buffer(&in_stream, (uint8_t*)buffer, sizeof(buffer));
    deserialize(&p2, &in_stream, ser_point_type);
    ser_stream_free(&in_stream);

    printf("Deserialized string: %s\n", p2.str);
    return 0;
}

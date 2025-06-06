#include "ser.h"
#include "ser_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    float va[3];
    char c;
} SubTest;

typedef struct {
    char ptr;
    int *ptr2;
} Test;

int main() {
    ser_type_t* ser_point_type = ser_struct(2,
        "ptr", offsetof(Test, ptr), ser_char(),
        "ptr2", offsetof(Test, ptr2), ser_pointer(ser_int())
    );

    Test p;
    p.ptr = 'Z';
    int a = 42;
    p.ptr2 = &a;

    size_t size;
    char *buffer = ser_serialize_to_dynamic_buffer(&p, ser_point_type, &size);

    Test p2 = {0};
    SerStream in_stream;
    ser_stream_init_buffer(&in_stream, (uint8_t*)buffer, size);
    deserialize(&p2, &in_stream, ser_point_type);
    ser_stream_free(&in_stream);

    printf("p2.ptr: %c\n", p2.ptr);
    if (p2.ptr2) {
        printf("p2.ptr2: %d\n", *p2.ptr2);
    } else {
        printf("p2.ptr2 is NULL\n");
    }
    return 0;
}

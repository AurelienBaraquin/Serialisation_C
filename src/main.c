#include "ser.h"
#include "ser_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef struct {
    float va[3];
    int *ptr;
    char c;
} SubTest;

int main() {
    ser_type_t* ser_sub_type = ser_primitive("SubTest", sizeof(SubTest));

    int a = 42;

    char *buf = ser_serialize_to_dynamic_buffer(&(SubTest){.va = {1.0f, 2.0f, 3.0f}, .ptr = &a, .c = 'a'}, ser_sub_type, NULL);

    SubTest sub;
    SerStream stream;
    ser_stream_init_buffer(&stream, (uint8_t*)buf, sizeof(SubTest));
    deserialize(&sub, &stream, ser_sub_type);
    ser_stream_free(&stream);

    printf("Deserialized SubTest: va = [%f, %f, %f], c = '%c'\n", sub.va[0], sub.va[1], sub.va[2], sub.c);
    if (sub.ptr) {
        printf("Pointer value: %d\n", *sub.ptr);
    } else {
        printf("Pointer is NULL\n");
    }
    ser_free(ser_sub_type);
    free(buf);

    return 0;
}

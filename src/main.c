#include "ser.h"
#include "ser_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef struct {
    float va[3];
    char c;
} SubTest;

typedef struct {
    SubTest sub[3];
    SubTest *sub_ptr[3];
    char ptr;
    int **ptr2;
    char *str;
} Test;

int main() {
    ser_type_t* ser_sub_type = ser_primitive(sizeof(SubTest));
    size_t str_size = 10;
    ser_type_t* ser_point_type = ser_struct(4,
        "sub", offsetof(Test, sub), ser_array(ser_sub_type, 3),
        "sub_ptr", offsetof(Test, sub_ptr), ser_array(ser_pointer(ser_sub_type), 3),
        "ptr", offsetof(Test, ptr), ser_char(),
        "ptr2", offsetof(Test, ptr2), ser_pointer(ser_pointer(ser_int()))
    );

    Test p;
    for (int i = 0; i < 3; i++) {
        p.sub[i].va[0] = 1.0f + i;
        p.sub[i].va[1] = 2.0f + i;
        p.sub[i].va[2] = 3.0f + i;
        p.sub[i].c = 'A' + i;
    }
    SubTest *sub_ptrs[3] = {&p.sub[0], &p.sub[1], &p.sub[2]};
    p.sub_ptr[0] = sub_ptrs[0];
    p.sub_ptr[1] = NULL;
    p.sub_ptr[2] = sub_ptrs[2];
    p.ptr = 'Z';
    int a = 42;
    int *b = &a;
    p.ptr2 = &b;
    p.str = malloc(str_size);
    strcpy(p.str, "Helloaaaa");

    size_t size;
    char *buffer = ser_serialize_to_dynamic_buffer(&p, ser_point_type, &size);

    Test p2 = {0};
    SerStream in_stream;
    ser_stream_init_buffer(&in_stream, (uint8_t*)buffer, size);
    deserialize(&p2, &in_stream, ser_point_type);
    ser_stream_free(&in_stream);

    for (int i = 0; i < 3; i++) {
        printf("p2.sub[%d].va: [%f, %f, %f]\n", i, p2.sub[i].va[0], p2.sub[i].va[1], p2.sub[i].va[2]);
        printf("p2.sub[%d].c: %c\n", i, p2.sub[i].c);
    }
    for (int i = 0; i < 3; i++) {
        if (p2.sub_ptr[i]) {
            printf("p2.sub_ptr[%d] points to: [%f, %f, %f], %c\n", i, p2.sub_ptr[i]->va[0], p2.sub_ptr[i]->va[1], p2.sub_ptr[i]->va[2], p2.sub_ptr[i]->c);
        } else {
            printf("p2.sub_ptr[%d] is NULL\n", i);
        }
    }
    printf("p2.ptr: %c\n", p2.ptr);
    if (p2.ptr2) {
        if (*p2.ptr2) {
            printf("p2.ptr2 points to: %d\n", **p2.ptr2);
        } else {
            printf("p2.ptr2 points to NULL\n");
        }
    } else {
        printf("p2.ptr2 is NULL\n");
    }
    // printf("p2.str: %s\n", p2.str ? p2.str : "NULL");

    return 0;
}

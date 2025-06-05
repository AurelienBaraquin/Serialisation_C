#include "ser.h"
#include "ser_primitive.h"
#include "ser_macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    char* name;
    Point location;
} Place;

#define __STRUCT_TYPE__ Point
SER_DECLARE_DEFINE(Point,
    SER_FIELD(x, ser_int),
    SER_FIELD(y, ser_int)
)
#undef __STRUCT_TYPE__

#define __STRUCT_TYPE__ Place
SER_DECLARE_DEFINE(Place,
    SER_FIELD(name, ser_string),
    SER_FIELD(location, ser_Point)
)
#undef __STRUCT_TYPE__



#define ARRAY_LEN 10

typedef struct {
    Place values[ARRAY_LEN];
} Vector3;

SER_DECLARE(Vector3);




typedef struct {
    char* name[ARRAY_LEN];
    float* age[ARRAY_LEN]; // optionnel
    Place* location[ARRAY_LEN]; // optionnel
} PersonOpt;

SER_DECLARE(PersonOpt)


int main() {
    ser_type_t *ser_opt = ser_optional(&ser_float);
    ser_type_t *ser_str_array = ser_array(&ser_string, ARRAY_LEN);
    ser_type_t *ser_opt_array = ser_array(ser_opt, ARRAY_LEN);
    ser_type_t *ser_opt_place = ser_optional(&ser_Place);
    ser_type_t *ser_opt_place_array = ser_array(ser_opt_place, ARRAY_LEN);
    #define __STRUCT_TYPE__ PersonOpt
    SER_DEFINE(PersonOpt,
        SER_FIELD(name, *ser_str_array),
        SER_FIELD(age, *ser_opt_array),
        SER_FIELD(location, *ser_opt_place_array)
    )
    #undef __STRUCT_TYPE__

    Place places[ARRAY_LEN] = {
        {"Place1", {1, 2}},
        {"Place2", {3, 4}},
        {"Place3", {5, 6}},
        {"Place4", {7, 8}},
        {"Place5", {9, 10}},
        {"Place6", {11, 12}},
        {"Place7", {13, 14}},
        {"Place8", {15, 16}},
        {"Place9", {17, 18}},
        {"Place10", {19, 20}}
    };

    float age = 25.5f;
    PersonOpt person = {
        .name = {"John", "Doe", "Alice", "Bob", "Charlie",
                 "Eve", "Frank", "Grace", "Heidi", "Ivan"},
        .age = {&age, NULL, NULL, NULL, NULL,
                 NULL, &age, NULL, &age, NULL},
        .location = {
            NULL, &places[1], NULL, &places[3], &places[4],
            &places[5], NULL, &places[7], NULL, NULL
        }
    };

    size_t size = 0;
    uint8_t* buffer = ser_serialize_to_dynamic_buffer(&person, &ser_PersonOpt, &size);

    SerStream in_stream;
    ser_stream_init_buffer(&in_stream, buffer, size);

    PersonOpt person2 = {0};
    deserialize(&person2, &in_stream, &ser_PersonOpt);
    ser_stream_free(&in_stream);
    // fclose(in);
    printf("Deserialized PersonOpt:\n");
    for (int i = 0; i < ARRAY_LEN; i++) {
        printf("Name: %s, Age: ", person2.name[i]);
        if (person2.age[i]) {
            printf("%f\n", *person2.age[i]);
        } else {
            printf("NULL\n");
        }
        if (person2.location[i]) {
            printf("Location: %s (%d, %d)\n",
                   person2.location[i]->name,
                   person2.location[i]->location.x,
                   person2.location[i]->location.y);
        } else {
            printf("Location: NULL\n");
        }
    }
    ser_free(&person2, &ser_PersonOpt);
    free(ser_opt);
    free(ser_str_array);
    free(ser_opt_array);
    free(ser_opt_place);
    free(ser_opt_place_array);
    free(buffer);

    return 0;
}

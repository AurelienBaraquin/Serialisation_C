#include "ser.h"

void serialize(void* obj, SerStream* out, ser_type_t* type) {
    type->serialize(obj, out, type);
}

void deserialize(void* obj, SerStream* in, ser_type_t* type) {
    type->deserialize(obj, in, type);
}

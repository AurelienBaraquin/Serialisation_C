#ifndef SER_H
#define SER_H

#include "ser_type.h"

void serialize(void* obj, SerStream* out, ser_type_t* type);
void deserialize(void* obj, SerStream* in, ser_type_t* type);

void serialize_struct(void* ptr, SerStream* out, const ser_type_t* self);
void deserialize_struct(void* ptr, SerStream* in, const ser_type_t* self);
void free_struct(void* ptr, ser_type_t* self);

void ser_free(void* ptr, ser_type_t* type);

#endif

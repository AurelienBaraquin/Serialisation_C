#ifndef SER_H
#define SER_H

#include "ser_type.h"

void serialize(void* obj, SerStream* out, ser_type_t* type);
void deserialize(void* obj, SerStream* in, ser_type_t* type);

// This function serializes an object to a dynamically allocated buffer.
uint8_t* ser_serialize_to_dynamic_buffer(void* object, ser_type_t* type, size_t* out_size);

void serialize_struct(void* ptr, SerStream* out, const ser_type_t* self);
void deserialize_struct(void** ptr, SerStream* in, const ser_type_t* self);
void free_struct(ser_type_t* self);

void ser_free(ser_type_t* type);

#endif

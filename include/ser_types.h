#ifndef SER_PRIMITIVE_H
#define SER_PRIMITIVE_H

#include "ser_type.h"

// Primitive types for serialization

ser_type_t* ser_primitive(size_t size);

ser_type_t* ser_int();
ser_type_t* ser_float();
ser_type_t* ser_double();
ser_type_t* ser_bool();
ser_type_t* ser_char();


// Combinator types for serialization

ser_type_t* ser_array(ser_type_t* subtype, size_t count);
ser_type_t* ser_struct(size_t field_count, ...);
ser_type_t* ser_pointer(ser_type_t* subtype);


// Serialization and deserialization functions for primitive types

void serialize_ser_primitive(void* ptr, SerStream* out, const ser_type_t* self);
void deserialize_ser_primitive(void** ptr, SerStream* in, const ser_type_t* self);

#endif

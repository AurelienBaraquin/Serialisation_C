#ifndef SER_PRIMITIVE_H
#define SER_PRIMITIVE_H

#include "ser_type.h"

// Primitive types for serialization

ser_type_t* ser_int();
ser_type_t* ser_float();
ser_type_t* ser_double();
ser_type_t* ser_string();
ser_type_t* ser_bool();
ser_type_t* ser_char();
ser_type_t* ser_void();


// Combinator types for serialization

ser_type_t* ser_array(ser_type_t* subtype, size_t count);
ser_type_t* ser_optional(ser_type_t* subtype);
ser_type_t* ser_struct(size_t field_count, ...);
ser_type_t* ser_ptr(ser_type_t* subtype);

#endif

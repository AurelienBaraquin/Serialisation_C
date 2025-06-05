#ifndef SER_PRIMITIVE_H
#define SER_PRIMITIVE_H

#include "ser_type.h"

extern ser_type_t ser_int;
extern ser_type_t ser_float;
extern ser_type_t ser_double;
extern ser_type_t ser_bool;
extern ser_type_t ser_char;
extern ser_type_t ser_string;

ser_type_t* ser_array(ser_type_t* subtype, size_t count);
ser_type_t* ser_optional(ser_type_t* subtype);

#endif

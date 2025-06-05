#include "ser.h"

void ser_free(void* ptr, ser_type_t* type) {
    if (ptr && type && type->free)
        type->free(ptr, type);
}

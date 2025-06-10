#include "ser.h"
#include <stdlib.h>

void ser_free(ser_type_t* type) {
    printf("Freeing type: %s\n", type->name);
    if (type && type->free)
        type->free(type);
}

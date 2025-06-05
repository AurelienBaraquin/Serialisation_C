#ifndef SER_STREAM_H
#define SER_STREAM_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct SerStream SerStream;

typedef size_t (*SerWriteFn)(SerStream* stream, const void* data, size_t size);
typedef size_t (*SerReadFn)(SerStream* stream, void* data, size_t size);

typedef enum {
    SER_STREAM_FILE,
    SER_STREAM_BUFFER,
    SER_STREAM_COUNTER
} SerStreamMode;

struct SerStream {
    SerWriteFn write;
    SerReadFn read;
    void* context;
    SerStreamMode mode;
};

void ser_stream_init_file(SerStream* stream, FILE* file);
void ser_stream_init_buffer(SerStream* stream, uint8_t* buffer, size_t capacity);
void ser_stream_init_counter(SerStream* stream);

void ser_stream_free(SerStream* stream);

size_t ser_stream_get_count(SerStream* stream);

uint8_t* ser_stream_get_buffer(SerStream* stream);
size_t ser_stream_get_offset(SerStream* stream);

#endif

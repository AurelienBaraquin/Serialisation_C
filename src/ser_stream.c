#include "ser_stream.h"
#include <string.h>
#include <stdlib.h>

// ---------- FILE backend ----------

typedef struct {
    FILE* file;
} FileContext;

static size_t file_write(SerStream* stream, const void* data, size_t size) {
    FileContext* ctx = (FileContext*)stream->context;
    return fwrite(data, 1, size, ctx->file);
}

static size_t file_read(SerStream* stream, void* data, size_t size) {
    FileContext* ctx = (FileContext*)stream->context;
    return fread(data, 1, size, ctx->file);
}

void ser_stream_init_file(SerStream* stream, FILE* file) {
    FileContext* ctx = malloc(sizeof(FileContext));
    ctx->file = file;

    stream->context = ctx;
    stream->write = file_write;
    stream->read = file_read;
    stream->mode = SER_STREAM_FILE;
}

// ---------- Buffer backend ----------

typedef struct {
    uint8_t* buffer;
    size_t offset;
    size_t capacity;
} BufferContext;

static size_t buffer_write(SerStream* stream, const void* data, size_t size) {
    BufferContext* ctx = (BufferContext*)stream->context;
    if (ctx->offset + size > ctx->capacity)
        return 0;

    memcpy(ctx->buffer + ctx->offset, data, size);
    ctx->offset += size;
    return size;
}

static size_t buffer_read(SerStream* stream, void* data, size_t size) {
    BufferContext* ctx = (BufferContext*)stream->context;
    if (ctx->offset + size > ctx->capacity)
        return 0;

    memcpy(data, ctx->buffer + ctx->offset, size);
    ctx->offset += size;
    return size;
}

void ser_stream_init_buffer(SerStream* stream, uint8_t* buffer, size_t capacity) {
    BufferContext* ctx = malloc(sizeof(BufferContext));
    ctx->buffer = buffer;
    ctx->offset = 0;
    ctx->capacity = capacity;

    stream->context = ctx;
    stream->write = buffer_write;
    stream->read = buffer_read;
    stream->mode = SER_STREAM_BUFFER;
}

// ---------- Counter backend ----------

typedef struct {
    size_t count;
} CounterContext;

static size_t counter_write(SerStream* stream, const void* data, size_t size) {
    CounterContext* ctx = (CounterContext*)stream->context;
    ctx->count += size;
    return size;
}

static size_t counter_read(SerStream* stream, void* data, size_t size) {
    // Lecture nulle : on ne lit rien, on simule uniquement
    return size;
}

void ser_stream_init_counter(SerStream* stream) {
    CounterContext* ctx = malloc(sizeof(CounterContext));
    ctx->count = 0;

    stream->context = ctx;
    stream->mode = SER_STREAM_COUNTER;
    stream->write = counter_write;
    stream->read = counter_read;
}

size_t ser_stream_get_count(SerStream* stream) {
    if (stream->mode != SER_STREAM_COUNTER) return 0;
    CounterContext* ctx = (CounterContext*)stream->context;
    return ctx->count;
}



// ---------- Free function ----------

void ser_stream_free(SerStream* stream) {
    if (stream->context) {
        free(stream->context);
        stream->context = NULL;
    }
    stream->write = NULL;
    stream->read = NULL;
}

uint8_t* ser_stream_get_buffer(SerStream* stream) {
    BufferContext* ctx = (BufferContext*)stream->context;
    return ctx->buffer;
}

size_t ser_stream_get_offset(SerStream* stream) {
    BufferContext* ctx = (BufferContext*)stream->context;
    return ctx->offset;
}

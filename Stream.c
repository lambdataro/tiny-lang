#include <stdlib.h>
#include <string.h>
#include "Stream.h"
#include "Util.h"

static Stream *createStream(StreamType type);
static int fileStreamGetChar(Stream *stream);
static int stringStreamGetChar(Stream *stream);

Stream *createFileStream(const char *filename)
{
    Stream *stream = createStream(STREAM_FILE);
    stream->fp = fopen(filename, "r");
    if (!stream->fp) {
        destroyStream(stream);
        return NULL;
    }
    return stream;
}

Stream *createStringStream(const char *str)
{
    Stream *stream = createStream(STREAM_STRING);
    stream->str.buffer = allocAndCopyString(str);
    stream->str.length = strlen(stream->str.buffer);
    stream->str.index = 0;
    return stream;
}

static Stream *createStream(StreamType type)
{
    Stream *stream = allocAndCheck(sizeof(Stream));
    stream->type = type;
    return stream;
}

void destroyStream(Stream *stream)
{
    if (stream->type == STREAM_FILE) {
        fclose(stream->fp);
    }
    if (stream->type == STREAM_STRING) {
        free(stream->str.buffer);
    }
    free(stream);
}

int getChar(Stream *stream)
{
    switch (stream->type) {
    case STREAM_FILE:
        return fileStreamGetChar(stream);
    case STREAM_STRING:
        return stringStreamGetChar(stream);
    default:
        fprintf(stderr, "unknown stream type\n");
        exit(EXIT_FAILURE);
    }
}

static int fileStreamGetChar(Stream *stream)
{
    return fgetc(stream->fp);
}

static int stringStreamGetChar(Stream *stream)
{
    if (stream->str.index >= stream->str.length) {
        return EOF;
    }
    return stream->str.buffer[stream->str.index++];
}

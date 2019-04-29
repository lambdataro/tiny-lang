#include <stdlib.h>
#include "Stream.h"
#include "Util.h"

static Stream *createStream(StreamType type);

static Stream *createStream(StreamType type)
{
    Stream *stream = allocAndCheck(sizeof(Stream));
    stream->type = type;
    return stream;
}

Stream *createFileStream(const char *filename)
{
    Stream *stream = createStream(STREAM_FILE);
    stream->fp = fopen(filename, "r");
    if (!stream->fp) {
        destroyStream(stream);
        return NULL;
    }
    nextChar(stream);
    return stream;
}

void destroyStream(Stream *stream)
{
    if (stream->type == STREAM_FILE) {
        fclose(stream->fp);
    }
    free(stream);
}

int nextChar(Stream *stream)
{
    if (stream->type == STREAM_FILE) {
        stream->ch = fgetc(stream->fp);
        return stream->ch;
    }
    fprintf(stderr, "unknown stream type\n");
    exit(EXIT_FAILURE);
}

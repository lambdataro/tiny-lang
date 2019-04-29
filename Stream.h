#ifndef TINY_LANG_STREAM_H_INCLUDED
#define TINY_LANG_STREAM_H_INCLUDED

#include <stdio.h>

typedef enum {
    STREAM_FILE,
    STREAM_STRING
} StreamType;

typedef struct {
    StreamType type;
    union {
        FILE *fp;
        struct {
            char *buffer;
            size_t length;
            size_t index;
        } str;
    };
} Stream;

Stream *createFileStream(const char *filename);
Stream *createStringStream(const char *str);
void destroyStream(Stream *stream);
int getChar(Stream *stream);

#endif // TINY_LANG_STREAM_H_INCLUDED

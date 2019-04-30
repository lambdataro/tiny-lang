#include <stdio.h>
#include <stdlib.h>
#include "StringBuffer.h"
#include "Util.h"

#define STRING_BUFFER_INITIAL_SIZE 8

StringBuffer *createStringBuffer(void)
{
    StringBuffer *buf = allocAndCheck(sizeof(StringBuffer));
    buf->size = sizeof(char) * STRING_BUFFER_INITIAL_SIZE;
    buf->buf = buf->ptr = allocAndCheck(buf->size);
    return buf;
}

void destroyStringBuffer(StringBuffer *buf)
{
    free(buf->buf);
    free(buf);
}

void stringBufferAddChar(StringBuffer *buf, char ch)
{
    *buf->ptr++ = ch;
    if (buf->size - 1 < (size_t)(buf->ptr - buf->buf)) {
        size_t oldSize = buf->size;
        buf->size *= 2;
        buf->buf = realloc(buf->buf, buf->size);
        buf->ptr = buf->buf + oldSize;
    }
}

void stringBufferAddString(StringBuffer *buf, const char *str)
{
    const char *ptr = str;
    while (*ptr) {
        stringBufferAddChar(buf, *ptr++);
    }
}

char *stringBufferToString(StringBuffer *buf)
{
    *buf->ptr = '\0';
    return allocAndCopyString(buf->buf);
}

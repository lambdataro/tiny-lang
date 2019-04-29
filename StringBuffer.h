#ifndef TINY_LANG_STRINGBUFFER_H_INCLUDED
#define TINY_LANG_STRINGBUFFER_H_INCLUDED

#include <stddef.h>

typedef struct {
    size_t size;
    char *buf;
    char *ptr;
} StringBuffer;

StringBuffer *createStringBuffer(void);
void destroyStringBuffer(StringBuffer *buf);
void stringBufferAddChar(StringBuffer *buf, char ch);
char *stringBufferToString(StringBuffer *buf);

#endif // TINY_LANG_STRINGBUFFER_H_INCLUDED

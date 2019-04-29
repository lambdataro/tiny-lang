#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Util.h"
#include "StringBuffer.h"

void *allocAndCheck(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

char *allocAndCopyString(const char *srcStr)
{
    size_t length = strlen(srcStr) + 1;
    char *str = allocAndCheck(sizeof(char) * length);
    strcpy(str, srcStr);
    return str;
}

char *readConsoleUntilEnterTwice(void)
{
    StringBuffer *buffer = createStringBuffer();
    int prevCh = '\0';
    int ch = getchar();
    if (ch == EOF) {
        return NULL;
    }
    while (ch != EOF && !(ch == '\n' && prevCh == '\n')) {
        stringBufferAddChar(buffer, ch);
        prevCh = ch;
        ch = getchar();
    }
    char *str = stringBufferToString(buffer);
    destroyStringBuffer(buffer);
    return str;
}

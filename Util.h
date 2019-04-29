#ifndef TINY_LANG_UTIL_H_INCLUDED
#define TINY_LANG_UTIL_H_INCLUDED

#include <stddef.h>

void *allocAndCheck(size_t size);
char *allocAndCopyString(const char *srcStr);
char *readConsoleUntilEnterTwice(void);
void fprintIndent(FILE *file, int indent);

#endif // TINY_LANG_UTIL_H_INCLUDED

#ifndef TINY_LANG_UTIL_H_INCLUDED
#define TINY_LANG_UTIL_H_INCLUDED

#include <stdio.h>

void *allocAndCheck(size_t size);
char *allocAndCopyString(const char *srcStr);
void fprintIndent(FILE *file, int indent);

#endif // TINY_LANG_UTIL_H_INCLUDED

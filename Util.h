#ifndef TINY_LANG_UTIL_H_INCLUDED
#define TINY_LANG_UTIL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

void *allocAndCheck(size_t size);
char *allocAndCopyString(const char *srcStr);

#endif // TINY_LANG_UTIL_H_INCLUDED

#ifndef TINY_LANG_UTIL_H_INCLUDED
#define TINY_LANG_UTIL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

void *allocAndCheck(size_t size);
char *allocAndCopyString(const char *srcStr);
void runCommand(const char *command);
void fprintIndent(FILE *file, int indent);

#endif // TINY_LANG_UTIL_H_INCLUDED

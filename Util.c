#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

void *allocAndCheck(size_t size)
{
    void *ptr = malloc(sizeof(size));
    if (!ptr) {
        fprintf(stderr, "memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

#ifndef TINY_LANG_MEMORY_H_INCLUDED
#define TINY_LANG_MEMORY_H_INCLUDED

#include "Value.h"

struct MemorySlot;

typedef struct {
    unsigned int count;
    struct MemorySlot *next;
} MemorySlotHeader;

typedef struct MemorySlot {
    MemorySlotHeader header;
    Value value;
} MemorySlot;

typedef struct MemoryPool {
    size_t size;
    size_t index;
    MemorySlot *pool;
    MemorySlot *freeList;
} MemoryPool;

MemoryPool *createMemoryPool(void);
void destroyMemoryPool(MemoryPool *pool);
Value *allocValue(MemoryPool *pool);
void freeValue(MemoryPool *pool, Value *value);
void incrRefCount(Value *value);
void statistics(MemoryPool *pool);

#endif // TINY_LANG_MEMORY_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "Memory.h"
#include "Util.h"
#include "Value.h"

#define MEMORY_POOL_SIZE 10

MemoryPool *createMemoryPool(void)
{
    MemoryPool *pool = allocAndCheck(sizeof(MemoryPool));
    pool->size = MEMORY_POOL_SIZE;
    pool->index = 0;
    size_t byteSize = sizeof(MemorySlot)*pool->size;
    pool->pool = allocAndCheck(byteSize);
    pool->freeList = NULL;
    memset(pool->pool, 0, byteSize);
    return pool;
}

void destroyMemoryPool(MemoryPool *pool)
{
    free(pool->pool);
    free(pool);
}

Value *allocValue(MemoryPool *pool)
{
    MemorySlot *slot;
    if (!pool->freeList) {
        if (pool->index >= pool->size) {
            fprintf(stderr, "memory limit error\n");
            exit(EXIT_FAILURE);
        }
        slot = pool->pool+pool->index++;
    }
    else {
        slot = pool->freeList;
        pool->freeList = slot->header.next;
    }
    slot->header.count = 1;
    return &slot->value;
}

void freeValue(MemoryPool *pool, Value *value)
{
    uint8_t *ptr = (uint8_t *) value;
    ptr -= sizeof(MemorySlotHeader);
    MemorySlot *slot = (MemorySlot *) ptr;
    slot->header.count--;
    if (slot->header.count > 0) {
        return;
    }
    if (value->type == VALUE_ERROR || value->type == VALUE_STR) {
        free(value->strVal);
    }
    if (value->type == VALUE_PAIR) {
        freeValue(pool, value->pairVal.fstValue);
        freeValue(pool, value->pairVal.sndValue);
    }
    slot->header.next = pool->freeList;
    pool->freeList = slot;
}

void incrRefCount(Value *value)
{
    uint8_t *ptr = (uint8_t *) value;
    ptr -= sizeof(MemorySlotHeader);
    MemorySlot *slot = (MemorySlot *) ptr;
    slot->header.count++;
}

void statistics(MemoryPool *pool)
{
    printf("MemoryPool:\n");
    printf("  (slot) size: %d, index: %d",
        (int) pool->size,
        (int) pool->index);
    int headerSize = sizeof(MemorySlotHeader);
    int valueSize = sizeof(Value);
    int slotSize = sizeof(MemorySlot);
    printf("  headerSize: %d, valueSize: %d, slotSize: %d\n", headerSize, valueSize, slotSize);
    printf("  ----\n");
    int used = 0;
    for (size_t i = 0; i < pool->size; i++) {
        printf("  %02d(%d): ", (int) i, pool->pool[i].header.count);
        if (pool->pool[i].header.count) {
            used++;
            fprintValue(stdout, &pool->pool[i].value);
            printf("\n");
        }
        else {
            printf("free\n");
        }
    }
    printf("  ----\n");
    printf("  total %dB, used: %dB (%f%%)\n",
        (int) pool->size*slotSize,
        used*slotSize,
        used ? (double) used/(double) pool->size*100 : 0.0);

    MemorySlot *slot = pool->freeList;
    int len = 0;
    while (slot) {
        len++;
        slot = slot->header.next;
    }
    printf("  free list length = %d\n\n", len);
}

#include <hal/heap.h>
#include <hal/log.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct
{
  uintptr_t unusedMemoryStart;
  uint32_t unusedMemorySize;
} Local;

static Local local;

uintptr_t Heap_getUnusedMemoryStart()
{
  return local.unusedMemoryStart;
}

uint32_t Heap_getUnusedMemorySize()
{
  return local.unusedMemorySize;
}

void Heap_print(void)
{
  logInfo("Heap_print: not implemented.");
}

int Heap_getSize(int units)
{
  return (int)(0 / units);
}

int Heap_getFreeSize(int units)
{
  return (int)(0 / units);
}

void Heap_init()
{
  local.unusedMemorySize = 487 * 1024 * 1024;
  local.unusedMemoryStart = (uintptr_t)malloc(local.unusedMemorySize);
}

void *Heap_memalign(size_t align, size_t size)
{
  return memalign(align, size);
}

void *Heap_malloc(size_t size)
{
  return malloc(size);
}

void *Heap_calloc(size_t nmemb, size_t size)
{
  return calloc(nmemb, size);
}

void *Heap_realloc(void *ptr, size_t size)
{
  return realloc(ptr, size);
}

void Heap_free(void *ptr)
{
  free(ptr);
}

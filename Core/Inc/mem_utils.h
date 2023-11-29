#include "FreeRTOS.h"
#include <stdlib.h>
#include <string.h>

__attribute__((annotate("to_duplicate"))) void memcpy_to_duplicate(void *__restrict__ __dest, const void *__restrict__ __src, size_t __n);

__attribute__((annotate("to_duplicate"))) void memset_to_duplicate(void *__restrict__ __dest, const int __src, size_t __n);

__attribute__((annotate("to_duplicate"))) void * pvPortMalloc_to_duplicate( size_t xWantedSize );

__attribute__((annotate("to_duplicate"))) void vPortFree_to_duplicate(void *pv);
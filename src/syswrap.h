/** @file System functions wrappers */

#ifndef _SYSWRAP_H
#define _SYSWRAP_H

#include <stdlib.h>

/**
 * @brief Malloc wrapper with error check
 * @param size: Mem size
 * @return Pointer to memory
 */
void* _malloc(size_t size);

/**
 * @brief Calloc wrapper
 * @param nmemb: Count
 * @param size: Size
 * @return
 */
void* _calloc(size_t nmemb, size_t size);

#endif /* _SYSWRAP_H */

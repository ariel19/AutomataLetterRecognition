#ifndef _SYSWRAP_H
#define _SYSWRAP_H

#include <stdlib.h>

void* _malloc(size_t size);
void* _calloc(size_t nmemb, size_t size);

#endif /* _SYSWRAP_H */

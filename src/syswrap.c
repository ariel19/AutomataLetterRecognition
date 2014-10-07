#include "syswrap.h"
#include "types.h"

void* _malloc(size_t size) {
	void *buf;
	if (!(buf = malloc(size)))
		ERR_PANIC("malloc");
	return buf;
}

void* _calloc(size_t nmemb, size_t size) {
	void *buf;
	if (!(buf = calloc(nmemb, size)))
		ERR_PANIC("calloc");
	return buf;
}

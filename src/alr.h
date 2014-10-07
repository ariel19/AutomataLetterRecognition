#ifndef _ALR_H
#define _ALR_H

#include "types.h"

typedef struct automata {
	unsigned char dummy;
} automata_t;

/**
 * @brief finds minimum from two specified  values
 * @param v1 first value
 * @param v2 second value
 * 
 * @return minimum from two values
**/
melem_t min(melem_t v1, melem_t v2);

/**
 * @brief finds maximum from the specified vector
 * @param vec1 vector of elements
 * @param size size of vector
 * 
 * @return maximum value from vector
**/
melem_t max(mvec1_t vec1, msize_t size);

#endif /* _ALR_H */

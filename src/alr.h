#ifndef _ALR_H
#define _ALR_H

#include "types.h"

typedef struct automata {
	unsigned char dummy;
} automata_t;

/**
 * MATRIX ADD & MUL FUNC
 */

/**
 * @brief finds minimum from two specified  values
 * @param v1 first value
 * @param v2 second value
 * 
 * @return minimum from two values
 */
melem_t min(const melem_t v1, const melem_t v2);

/**
 * @brief finds maximum from the specified vector
 * @param vec1 vector of elements
 * @param size size of vector
 * 
 * @return maximum value from vector
 */
melem_t max(const mvec1_t vec1, const msize_t size);

/**
 * FEATURE
 */

/**
 * @brief responsible for normalizing specified feature vector
 * @
 */
ftr_err_code normalize(feat_t feat, const feat_t normal, const fsize_t size);

#endif /* _ALR_H */

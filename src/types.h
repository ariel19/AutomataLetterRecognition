#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>
#include <stdio.h>

#define MTX_2D(i, j, n) (i * n + j)

/**
 * MATRIX
 */
typedef uint32_t	msize_t; /* matrix size type */
typedef uint8_t		melem_t; /* matrix element type */
typedef melem_t*	mvec1_t; /* vector type */
typedef melem_t*	mvec2_t; /* 2d matrix */
typedef melem_t**	mvec3_t; /* 3d matrix */

typedef melem_t 	(*mfunc_mul)(melem_t, melem_t);
typedef melem_t 	(*mfunc_add)(mvec1_t, msize_t);\

/**
 * FEATURE 
 */
typedef uint32_t	fsize_t; /* feature size type */
typedef double		felem_t; /* feature value */
typedef felem_t*	feat_t;	 /* array of feature */	

typedef uint8_t		err_t;	 /* error type */

/**
 * ERROR CODES
 **/

#define ERR_PANIC(src) (fprintf(stderr, "%s: %d", __FILE__, __LINE__), \
						perror(src), exit(EXIT_FAILURE))
 
 /* matrix error codes */
typedef enum { 	
				MTX_OK = 0, 
				MTX_DIMENSION_POS, 	/* dimension should be positive */
				MTX_DIMENSION_OFR, 	/* dimension is out of range */
				MTX_DIMENSION_DIFF,	/* dimension differs (should be equal) */
				MTX_IS_NULL,		/* internal matrix is NULL */
				MTX_STRUCT_IS_NULL,	/* mtx pointer is NULL */
				MTX_OP_IS_NULL		/* add && | || is NULL */
			} mtx_err_code;

/* feature error codes */		
typedef enum {
				FTR_OK = 0
			} ftr_err_code;
			


#endif /* _TYPES_H */

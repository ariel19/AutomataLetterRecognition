#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>
#include <stdio.h>

/**
 * ERROR CODES
 **/

#define ERR_PANIC(src) (fprintf(stderr, "%s: %d", __FILE__, __LINE__), \
						perror(src), exit(EXIT_FAILURE))

typedef enum {
				SYM_A,
				SYM_B,
				SYM_C,
				SYM_D,
				SYM_E,
				SYM_F,
				SYM_G,
				SYM_H,
				SYM_I,
				SYM_J,
				SYM_K,
				SYM_L,
				SYM_M,
				SYM_N,
				SYM_O,
				SYM_P,
				SYM_Q,
				SYM_R,
				SYM_S,
				SYM_T,
				SYM_U,
				SYM_V,
				SYM_W,
				SYM_X,
				SYM_Y,
				SYM_Z
			} symbol_class;

 /* matrix error codes */
typedef enum { 	
				MTX_OK = 0, 
				MTX_DIMENSION_POS, 	/* dimension should be positive */
				MTX_DIMENSION_OFR, 	/* dimension is out of range */
				MTX_DIMENSION_DIFF,	/* dimension differs (should be equal) */
				MTX_IS_NULL,		/* internal matrix is NULL */
				MTX_STRUCT_IS_NULL,	/* mtx pointer is NULL */
				MTX_OP_IS_NULL,		/* add && | || is NULL */
				MTX_INVALID_INDEX
			} mtx_err_code;

/* feature error codes */		
typedef enum {
				FTR_OK = 0,
				FTR_STRUCT_IS_NULL, /* ftr pointer is NULL */
				FTR_IS_NULL,		/* internal vector is NULL */
				FTR_DIMENSION_DIFF  /* dimension differs (should be equal) */
			} ftr_err_code;
			
typedef enum {
				ATM_OK = 0,
				ATM_STRUCT_IS_NULL, /* atm pointer is NULL */
				ATM_MAX_IS_NULL,	/* max pointer is NULL */
				ATM_RANGE_IS_NULL,
				ATM_SPLIT_POS,		/* split number should be positive */
				ATM_MTX_INIT_ERR,
				ATM_SCN_POS,			/* symbol class number should be positive */
				ATM_INTERNAL_ERROR 	/* ;) */
			} atm_err_code;

/**
 * MATRIX
 */

#define MTX_2D(i, j, n) (i * n + j)

typedef uint32_t	msize_t; /* matrix size type */
#ifdef FUZZY_TYPE
	typedef double		melem_t; /* matrix element type */
#else
	typedef uint8_t		melem_t; /* matrix element type */
#endif
typedef melem_t*	mvec1_t; /* vector type */
typedef melem_t*	mvec2_t; /* 2d matrix */
typedef melem_t**	mvec3_t; /* 3d matrix */

typedef melem_t 	(*mfunc_mul)(melem_t, melem_t);
typedef melem_t 	(*mfunc_add)(mvec1_t, msize_t);

/**
 * FEATURE 
 */
 
typedef uint32_t	fsize_t; /* feature size type */
typedef double		felem_t; /* feature value */
typedef felem_t*	feat_t;	 /* vector of features */	
typedef symbol_class* fscn_t;/* vector of mapped splits */
typedef fsize_t*	srdet_t; /* vector of splited deterministic values */

typedef uint8_t		err_t;	 /* error type */

/**
 * AUTOMATA
 */
typedef uint32_t	lssize_t; /* learn set size and number of errors type */
typedef uint8_t		bool_t; /* boolean type */

typedef struct pso_params {
    uint32_t iterations;
    uint32_t swarmsize;
    uint32_t trace;
    double fnscale;
    double w;
    double cp;
    double cg;
} pso_params_t;

#endif /* _TYPES_H */

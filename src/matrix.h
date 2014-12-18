/** @file Matrix definition header */

#ifndef _MATRIX_H
#define _MATRIX_H

#include "types.h"
#include "syswrap.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief 3d matrix structure
 * m 	:= split
 * n, k := states
 * add	:= add operator handler
 * mul	:= mul operator handler
 **/
typedef struct matrix {
	mfunc_add add;
	mfunc_mul mul;
	
	mvec3_t mtx;
	msize_t m, n, k;
} matrix_t;

/**
 * 2d/3d matrix functions
 **/

/**
 * @brief initializes matrix with specified args
 * @param mtx	pointer to matrix_t structure
 * @param m	third dimension
 * @param n	second dimension
 * @param k	first dimension
 * 
 * @return MTX_OK if function succeed, error code in other cases
**/
mtx_err_code matrix_ptr_check(const matrix_t *mtx);
 
 /**
  * @brief initializes matrix with specified args
  * @param mtx	pointer to matrix_t structure
  * @param m	third dimension
  * @param n	second dimension
  * @param k	first dimension
  * 
  * @return MTX_OK if function succeed, error code in other cases
 **/  
mtx_err_code matrix_init(matrix_t *mtx, const msize_t m, const msize_t n, const msize_t k);
 
 /**
  * @brief randomly sets value one in every column of 2d array (get from split m)
  * @param mtx pointer to matrix_t structure
  * 
  * @return MTX_OK if function succeed, error code in other cases
 **/
mtx_err_code matrix_set_cols(matrix_t *mtx);

 /**
  * @brief frees and cleans memory in specified matrix
  * @param mtx pointer to matrix_t structure
  * 
  * @return MTX_OK if function succeed, error code in other cases
 **/
mtx_err_code matrix_free(matrix_t *mtx);

 /**
  * @brief sets a function responsible for add operation in matrix multiplication
  * @param mtx pointer to matrix_t structure
  * @param add pointer to add operation function
  * 
  * @return MTX_OK if function succeed, error code in other cases
 **/
mtx_err_code matrix_set_add(matrix_t *mtx, const mfunc_add add);

/**
  * @brief sets a function responsible for mul operation in matrix multiplication
  * @param mtx pointer to matrix_t structure
  * @param mul pointer to mul operation function
  * 
  * @return MTX_OK if function succeed, error code in other cases
 **/
mtx_err_code matrix_set_mul(matrix_t *mtx, const mfunc_mul mul);

/**
  * @brief matrix x vector multiplication function
  * @param mtx pointer to matrix_t structure
  * @param m split value
  * @param vec1 vector
  * @param vec_size lenght of vector
  * @param vec2(out) pointer to mvec2_t filled with output multiplication vector values (!!!SHOULD BE FREED!!!)
  * 
  * @return MTX_OK if function succeed, error code in other cases
 **/
mtx_err_code matrix_mul(const matrix_t *mtx, const msize_t m, const mvec1_t vec1, const msize_t vec_size, mvec2_t *vec2);

/**
  * @brief gets a 2d matrix from 3d matrix using specified split value
  * @param mtx pointer to matrix_t structure
  * @param m split value
  * @param vec2(out) pointer to mvec2_t, that will point to 2d matrix
  * 
  * @return MTX_OK if function succeed, error code in other cases
 **/
mtx_err_code matrix_get_2d(const matrix_t *mtx, const msize_t m, mvec2_t *vec2);

 /**
  * @brief sets a value on specified position
  * @param mtx pointer to matrix_t structure
  * @param m split value
  * @param n second dimension
  * @param k first dimension
  * @param val value to set
  * 
  * @return MTX_OK if function succeed, error code in other cases
 **/
mtx_err_code matrix_set_val(matrix_t *mtx, const msize_t m, const msize_t n, const msize_t k, const melem_t val);

/**
  * @brief shows matrix in human readable format
  * @param mtx pointer to matrix_t structure
  * @param m split value
  * 
  * @return MTX_OK if function succeed, error code in other cases
 **/
mtx_err_code matrix_show(const matrix_t *mtx, const msize_t m);
 
 #endif /* _MATRIX_H */
 

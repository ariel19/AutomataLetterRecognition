#include "matrix.h"

/* initializes matrix with specified args */
mtx_err_code matrix_ptr_check(matrix_t *mtx) {
	if (!mtx)
		return MTX_STRUCT_IS_NULL;
		
	if (!mtx->mtx)
		return MTX_IS_NULL;
	return MTX_OK;
}

/* initializes matrix with specified args */
mtx_err_code matrix_init(matrix_t *mtx, msize_t m, msize_t n, msize_t k) {
	msize_t i;
	
	if (!mtx)
		return MTX_STRUCT_IS_NULL;
	
	mtx->mtx = NULL;
	mtx->add = NULL;
	mtx->mul = NULL;
	mtx->m = mtx->n = mtx->k = 0;
	
	if (!(m && n && k))
		return MTX_DIMENSION_POS;
	
	/* allocate memory for 3d */
	mtx->mtx = (mvec3_t)_calloc(m, sizeof(mvec2_t));
	
	/* allocate memory for 2d */
	for (i = 0; i < m; ++i)
		mtx->mtx[i] = (mvec2_t)_calloc(n * k, sizeof(melem_t));
	
	/* set sizes */
	mtx->m = m;
	mtx->n = n;
	mtx->k = k;
		
	return MTX_OK;
}

/* randomly sets value one in every column of 2d array (get from split m) */
mtx_err_code matrix_set_cols(matrix_t *mtx) {
	msize_t m, k;
	mvec2_t vec2;
	mtx_err_code ret;
	
	if ((ret = matrix_ptr_check(mtx)))
		return ret;
		
	for (m = 0; m < mtx->m; ++m)
		for (k = 0; k < mtx->k; ++k) {
			/* TODO: add new error code here */
			if ((ret = matrix_get_2d(mtx, m, &vec2)))
				return ret;
			vec2[MTX_2D(rand() % mtx->n, k, mtx->k)] = 1;
		}
			
	return MTX_OK;
}

/* frees and cleans memory in specified matrix */
mtx_err_code matrix_free(matrix_t *mtx) {
	msize_t i;
	mtx_err_code ret;
	
	if ((ret = matrix_ptr_check(mtx)))
		return ret;
	
	/* free 2d matrices */
	for (i = 0; i < mtx->m; ++i)
		if (mtx->mtx[i])
			free(mtx->mtx[i]);
			
	memset(mtx->mtx, 0, mtx->m * sizeof(mvec2_t));
	
	/* free 3d matrix */
	free(mtx->mtx);
	mtx->mtx = NULL;
	
	mtx->m = mtx->n = mtx->k = 0;
	mtx->add = NULL;
	mtx->mul = NULL;
	
	return MTX_OK;	
}

/* sets a function responsible for add operation in matrix multiplication */
mtx_err_code matrix_set_add(matrix_t *mtx, mfunc_add add) {
	if (!mtx)
		return MTX_STRUCT_IS_NULL;
	
	mtx->add = add;	
	
	return MTX_OK;
}

/* sets a function responsible for mul operation in matrix multiplication */
mtx_err_code matrix_set_mul(matrix_t *mtx, mfunc_mul mul) {
	if (!mtx)
		return MTX_STRUCT_IS_NULL;
	
	mtx->mul = mul;
	
	return MTX_OK;
}

/* matrix x vector multiplication function */
mtx_err_code matrix_mul(matrix_t *mtx, msize_t m, mvec1_t vec1,  msize_t vec_size, mvec2_t *vec2) {
	mvec2_t _vec2;
	mvec1_t mul;
	mtx_err_code ret;
	msize_t _n, _k;
	
	if ((ret = matrix_ptr_check(mtx)))
		return ret;
	
	if ((ret = matrix_get_2d(mtx, m, &_vec2)))
		return ret;
		
	if (mtx->k != vec_size)
		return MTX_DIMENSION_DIFF;
	
	if (!(mtx->add && mtx->mul))
		return MTX_OP_IS_NULL;
		
	/* TODO: if vec1 is NULL notify */
	if (!vec1)
		return MTX_OK;
	
	/* TODO: check _vec2 */
	if (!vec2)
		return MTX_OK;
	
	/* TODO: check n, k, really idk if we have to do that, cause mtx size is initialized at the end of init see matrix_init*/
	*vec2 = (mvec2_t)_calloc(mtx->n, sizeof(melem_t));
	mul = (mvec1_t)_calloc(mtx->k, sizeof(melem_t));
	
	/* multiply matrix x vector using predefined add & mul */
	for (_n = 0; _n < mtx->n; ++_n) {
		for (_k = 0; _k < mtx->k; ++_k)
			mul[_k] = mtx->mul(_vec2[MTX_2D(_n, _k, mtx->k)], vec1[_k]); /* currently min operatior */
		(*vec2)[_n] = mtx->add(mul, mtx->k);
	}
	
	free(mul);
	
	return MTX_OK;
}

/* gets a 2d matrix from 3d matrix using specified split value */
mtx_err_code matrix_get_2d(matrix_t *mtx, msize_t m, mvec2_t *vec2) {
	mtx_err_code ret;
	
	if ((ret = matrix_ptr_check(mtx)))
		return ret;
	
	if (mtx->m <= m)
		return MTX_DIMENSION_OFR;
	
	/* TODO: do we need to notify error in case if vec2 is NULL */
	if (vec2)
		*vec2 = mtx->mtx[m];
		
	return MTX_OK;
}

/* sets a value on specified position */
mtx_err_code matrix_set_val(matrix_t *mtx, msize_t m, msize_t n, msize_t k, melem_t val) {
	mtx_err_code ret;
	mvec2_t vec2;
	
	if ((ret = matrix_get_2d(mtx, m, &vec2)))
		return ret;
		
	/* TODO: add a new error code here */
	if (mtx->n <= n || mtx->k <= k)
		return MTX_OK;
		
	vec2[MTX_2D(n, k, mtx->k)] = val;
		
	return MTX_OK;
}

/* shows matrix in human readable format */
mtx_err_code matrix_show(matrix_t *mtx, msize_t m) {
	mtx_err_code ret;
	msize_t n, k;
	mvec2_t vec2;
	
	if ((ret = matrix_get_2d(mtx, m, &vec2)))
		return ret;
		
	fprintf(stdout, "[\n");
	for (n = 0; n < mtx->n; ++n) {
		fprintf(stdout, "\t");
		for (k = 0; k < mtx->k; ++k)
			fprintf(stdout, "%.1d ", vec2[MTX_2D(n, k, mtx->k)]);
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "]\n");
		
	return MTX_OK;
}

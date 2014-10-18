#ifndef _ALR_H
#define _ALR_H

#include "types.h"
#include "matrix.h"

/**
 * @brief structure that represents feature vector
 * feat feature values vector
 * size vector size
 * correct correct state
 * determin_splits vector of deterministic splits A_1, A_3 .... A_6, A_2
 */
typedef struct feature {
	feat_t feat;
	fsize_t size;
	symbol_class correct;
	srdet_t determin_splits;
} feature_t;

/**
 * @brief structure that represents statistics of correctly distinguished symbols
 * whole whole number of input features
 * errors number of errors during the automata work
 */
typedef struct statistic {
	lssize_t whole,
			 errors;
} statistic_t;

/**
 * @brief structure that represents dynamically generated automata
 * feat feature structure
 * mtx matrix structure
 * stat statistic structure
 * rng range structure
 * state current state
 * splits number of splits on [0, 1]
 * sym_class_num number of symbol classes
 */
typedef struct automata {
	feature_t feat;
	feature_t max;
	matrix_t mtx;
	statistic_t stat;
	symbol_class state;
	msize_t splits,
			sym_class_num;
	feat_t	range;
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
 * AUTOMATA
 */

/**
 * @brief initializes a specified automata instance
 * @param atm pointer to automata_t
 * @param splits number of splits in range [0, 1]
 * @param sym_class_num number of symbol classes
 * 
 * @return ATM_OK if function succeed, error code in other cases
 */
atm_err_code automata_init(automata_t *atm, const feat_t *max, const fsize_t feature_num, const msize_t splits, const msize_t sym_class_num);

void init_from_vec(double *vec, automata_t *atm);

void automata_free(automata_t *atm);

/**
 * @brief initializes a matrix related with specified automata
 * @param atm pointer to automata_t
 * 
 * @return ATM_OK if function succeed, error code in other cases
 */
atm_err_code automata_init_matrix(automata_t *atm);

/**
 * @brief starts automata building
 * @param atm pointer to automata_t
 * 
 * @return ATM_OK if function succeed, error code in other cases
 */
atm_err_code automata_build_start(automata_t *atm, msize_t input_size, feature_t *features, msize_t repeat);

void automata_build(double *vec, automata_t *atm, msize_t input_size, feature_t *features, msize_t *err_num);

/**
 * @brief maps real values -> deterministic
 * @param atm pointer to automata_t
 * 
 * @return ATM_OK if function succeed, error code in other cases
 */
atm_err_code automata_split_range(automata_t *atm);

/**
 * FEATURE
 */

/**
 * @brief responsible for normalizing specified feature vector
 * @param atm pointer to automata_t
 * 
 * @return ATM_OK if function succeed, error code in other cases
 */
ftr_err_code automata_feature_normalize(automata_t *atm, feature_t *feat);

#endif /* _ALR_H */

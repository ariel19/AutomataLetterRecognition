/** @file Automata definition header */

#ifndef _ALR_H
#define _ALR_H

#include "types.h"
#include "matrix.h"

int test_run;

/**
 * @brief Structure that represents feature vector
 * feat: Feature values vector
 * size: Vector size
 * correct: Correct state
 * determin_splits: Vector of deterministic splits A_1, A_3 .... A_6, A_2
 */
typedef struct feature {
	feat_t feat;
	fsize_t size;
	symbol_class correct;
	srdet_t determin_splits;
} feature_t;

/**
 * @brief Structure that represents statistics of correctly distinguished symbols
 * whole: Whole number of input features
 * errors: Number of errors during the automata work
 * fuzzy_errors: Number of errors during the fuzzy automata work
 */
typedef struct statistic {
	lssize_t whole,
			 errors;
	double fuzzy_errors;
} statistic_t;

/**
 * @brief structure that represents dynamically generated automata
 * feat: Feature structure
 * mtx: Matrix structure
 * stat: Statistic structure
 * range: Range structure
 * state: Current state
 * splits: Number of splits on [0, 1]
 * sym_class_num: Number of symbol classes
 * max_los: Max feature value of generated data
 * min_los: Min feature value of generated data
 * min_tab: Min feature values of read data
 * max_tab: Max feature values of read data
 * is_read: Is data read from files?
 * fuzzy: Is automata fuzzy?
 */
typedef struct automata {
	feature_t feat;
    double max_los;
    double min_los;
    feature_t min_tab;
    feature_t max_tab;
    int is_read;
	matrix_t mtx;
	statistic_t stat;
	symbol_class state;
	msize_t splits,
			sym_class_num;
	feat_t	range;
	bool_t  fuzzy; /* is automata fuzzy */
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
 * @brief implementation of mul operator
 * @param v1 first value
 * @param v2 second value
 * 
 * @return composition function result
 */
melem_t amin(const melem_t v1, const melem_t v2);

/**
 * @brief implementation of plus operator
 * @param vec1 vector of elements
 * @param size size of vector
 *
 * @return composition function result
 */
melem_t amax(const mvec1_t vec1, const msize_t size);

/**
 * AUTOMATA
 */

/**
 * @brief Initializes automata instance
 * @param atm: Automata
 * @param is_read: Is data read from file?
 * @param max_los: Max value of generated feature
 * @param min_los: Min value of generated feature
 * @param min_tab: Min values of read features
 * @param max_tab: Max values of read features
 * @param feature_num: Number of features
 * @param splits: Number of [0..1] vector splits
 * @param sym_class_num: Number of symbols
 * @return
 */
atm_err_code automata_init(automata_t *atm, int is_read, double max_los, double min_los, feat_t *min_tab, feat_t *max_tab,
                           const fsize_t feature_num, const msize_t splits, const msize_t sym_class_num);

/**
 * @brief Initializes the automata matrix (integers) from pso optimized data
 * @param vec: PSO vector
 * @param atm: Automata
 * @param nondet_prop: Nondeterministic automata percentage limit
 */
void init_from_vec(double *vec, automata_t *atm, double nondet_prop);

/**
 * @brief Initializes the automata matrix (doubles) from pso optimized data
 * @param vec: PSO vector
 * @param atm: Automata
 */
void init_from_dvec(double *vec, automata_t *atm);

/**
 * @brief Frees the memory
 * @param atm: Automata
 */
void automata_free(automata_t *atm);

/**
 * @brief Initializes a matrix related with specified automata
 * @param atm: Pointer to automata_t
 *
 * @return ATM_OK if function succeed, error code in other cases
 */
atm_err_code automata_init_matrix(automata_t *atm);

/**
 * @brief Starts building of the automata
 * @param vec: Vector with data to initialize the automata matrix
 * @param atm: Automata
 * @param input_size: Number of symbols to test
 * @param features: Vector of features
 * @param err_num: Number of wrong recognized symbols
 * @param nondet_prop: Nondeterministic automata percentage limit
 */
void automata_build(double *vec, automata_t *atm, msize_t input_size, feature_t *features, double *err_num, double nondet_prop);

/**
 * @brief Maps real values -> [0..1]
 * @param atm: Pointer to automata_t
 *
 * @return ATM_OK if function succeed, error code in other cases
 */
atm_err_code automata_split_range(automata_t *atm);

/**
 * FEATURE
 */

/**
 * @brief Responsible for normalizing specified feature vector
 * @param atm: Pointer to automata_t
 *
 * @return ATM_OK if function succeed, error code in other cases
 */
ftr_err_code automata_feature_normalize(automata_t *atm, feature_t *feat);

/**
 * @brief Printing the automata matrix
 * @param atm: Automata
 */
void print_atm(automata_t *atm);

#endif /* _ALR_H */

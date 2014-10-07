#include "alr.h"

/* finds minimum from two specified  values */
melem_t min(const melem_t v1, const melem_t v2) {
	return v1 > v2 ? v2 : v1;
}

/* finds maximum from the specified vector */
melem_t max(const mvec1_t vec1, const msize_t size) {
	msize_t s;
	melem_t _max;
	/* TODO: if vec1 is NULL */
	if (!vec1)
		return 0;
	
	/* TODO: handle */
	if (size <= 0)
		return 0;
		
	_max = vec1[0];
	for (s = 1; s < size; ++s)
		if (_max < vec1[s])
			_max = vec1[s];
			
	return _max;
}

/* initializes a specified automata instance */
atm_err_code automata_init(automata_t *atm, const feat_t *max, const fsize_t feature_num, const msize_t splits, const msize_t sym_class_num) {
	atm_err_code ret;
	if (!atm)
		return ATM_STRUCT_IS_NULL;
		
	if (!splits)
		return ATM_SPLIT_POS;
		
	if (!sym_class_num)
		return ATM_SCN_POS;
		
	if(!max)
		return ATM_MAX_IS_NULL;
	
	/* init splits and symbol class number */
	atm->splits = splits;
	atm->sym_class_num = sym_class_num;
	
	/* init structures */
	memset(&(atm->feat), 0, sizeof(feature_t));
	memset(&(atm->stat), 0, sizeof(statistic_t));
	memset(&(atm->max), 0, sizeof(feature_t));
	atm->max.size = feature_num;
	atm->max.feat = *max;
	
	/* init range according to the split value */
	atm->range = (feat_t)_calloc(atm->splits + 1, sizeof(felem_t));
	
	if ((ret = automata_split_range(atm)))
		return ret;
	
	automata_init_matrix(atm);
	
	return ATM_OK;
}

/* receives next feature structure as an input */
atm_err_code automata_get_feat(automata_t *atm, feature_t *feat) {
	return ATM_OK;
}

/* starts automata building */
atm_err_code automata_build_start(automata_t *atm) {
	atm_err_code ret;
	fsize_t s;
	mvec1_t out_vec,
			cs_vec; /* current state vector */
	msize_t input_size = 3000, i; /* TODO: should be changed */
	symbol_class prev_state;
	
	/* choose a start state here */
	matrix_set_cols(&(atm->mtx));
	cs_vec = (mvec1_t)_calloc(atm->mtx.k, sizeof(melem_t));
	
	for (i = 0; i < input_size; ++i) {
		/* Get next feature using a automata_get_feat_func*/
		/* TODO: have to read feature vector form somewhere */
		/*
		if ((ret = automata_get_feat(atm, )))
			return ret;
		*/
		
		/* after this operation we will have a normalized vector in feat structure */
		if ((ret = automata_feature_normalize(atm)))
			return ret;
		
		/* after this operation we have a deterministic splits */
		if ((ret = automata_map_splits(atm)))
			return ret;
		
		atm->state = SYM_A;
		/* for each element in deterministic split vector */
		for (s = 0; s < atm->feat.size; ++s) {
			/* FUNC(current_state, SPLIT_VAL(i)) = next_state */
			/* current_state = next_state */
			prev_state = atm->state;
			cs_vec[atm->state] = 1;
			atm->state = matrix_mul(&(atm->mtx), atm->feat.determin_splits[s], cs_vec, atm->mtx.k, &out_vec);
			cs_vec[prev_state] = 0;
		 }
		 
		free(atm->feat.determin_splits);
		/* check if out state equals to correct state, if no increase errors+ */
		++atm->stat.whole;
		if (!out_vec[atm->feat.correct])
			++atm->stat.errors;
			
		free(out_vec);
		
	}
	
	/* TODO: clean memory but we have to remember about MATRIX*/
	free(cs_vec);
	
	/* FIXME: call a PSO here with atm->stat.whole, atm->stat.error and atm->mtx */
	
	
	/* With new matrix start computations once again (SHOULD BE A CONDITION)*/
	
	matrix_free(&(atm->mtx));
	free(atm->range);
	
	return ATM_OK;
}

/* maps real values -> deterministic */
atm_err_code automata_split_range(automata_t *atm) {
	msize_t s;
	if (!atm)
		return ATM_STRUCT_IS_NULL;
	
	if (!atm->range)
		return ATM_RANGE_IS_NULL;
	
	for (s = 0; s <= atm->splits; ++s)
		atm->range[s] = s / atm->splits;
	
	return ATM_OK;
}

/* initializes a matrix related with specified automata */
atm_err_code automata_init_matrix(automata_t *atm) {
	/* FIXME: DUMMY DATA */
	/*
	melem_t vec1[] = { 0, 1, 0, 0 };
	mvec1_t vec;
	msize_t s;
	*/
	
	mtx_err_code ret;
	
	/* should be NUM_OF_SPLITS x NUM_OF_SYM x NUM_OF_SYM */
	if ((ret = matrix_init(&(atm->mtx), atm->splits, atm->sym_class_num, atm->sym_class_num)))
		return ATM_MTX_INIT_ERR;
	if ((ret = matrix_set_add(&(atm->mtx), max)))
		return ATM_MTX_INIT_ERR;
	if ((ret = matrix_set_mul(&(atm->mtx), min)))
		return ATM_MTX_INIT_ERR;
	
	/*
	matrix_show(&(atm->mtx), 0);
	matrix_show(&(atm->mtx), 1);
	*/
	
	/* matrix_set_cols(&(atm->mtx)); */
	/*
	matrix_show(&(atm->mtx), 0);
	matrix_show(&(atm->mtx), 1);
	
	matrix_mul(&(atm->mtx), 0, vec1, 4, &vec);
	
	for (s = 0; s < 3; ++s)
		fprintf(stdout, "%.1d ", vec[s]);
	fprintf(stdout, "\n");

	matrix_mul(&(atm->mtx), 1, vec1, 4, &vec);
	
	for (s = 0; s < 3; ++s)
		fprintf(stdout, "%.1d ", vec[s]);
	fprintf(stdout, "\n");
	
	free(vec);
		
	matrix_free(&(atm->mtx));
	*/
	
	return ATM_OK;
}

/* responsible for normalizing specified feature vector */
ftr_err_code automata_feature_normalize(automata_t *atm) {
	fsize_t s;
	msize_t k;
	
	feature_t *feat = &(atm->feat);
	const feature_t *max = &(atm->max);
	
	if (!feat || !max)
		return FTR_STRUCT_IS_NULL;
		
	if (!feat->feat || !max->feat)
		return FTR_IS_NULL;
		
	if (feat->size != max->size)
		return FTR_DIMENSION_DIFF;
		
	for (s = 0; s < feat->size; ++s)
		feat->feat[s] /= max->feat[s];
		
	/* use splitted range i order to fill deterministica vector */
	/* TODO: error code */
	if (!atm->range)
		return 0;
	
	atm->feat.determin_splits = (srdet_t)_calloc(atm->feat.size, sizeof(fsize_t));
	for (s = 0; s < atm->feat.size; ++s) {
		for(k = 0; k < atm->splits; ++k) {
			if(atm->feat.feat[s] <= atm->range[k + 1]) {
				atm->feat.determin_splits[s] = k;
				break;
			}
			if(k == atm->splits - 1)
				atm->feat.determin_splits[s] = k;
		}
	}
	
	return FTR_OK;
}

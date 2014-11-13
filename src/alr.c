#include "alr.h"
#include "swarm.h"

#include <float.h>
#include <math.h>

/* finds minimum from two specified  values */
melem_t min(const melem_t v1, const melem_t v2) {
	return v1 > v2 ? v2 : v1;
}

/* finds maximum from the specified vector */
melem_t max(const mvec1_t vec1, const msize_t size) {
	msize_t s;
	melem_t _max;

	if (!vec1)
		return -1;

	if (size <= 0)
		return -1;

	_max = vec1[0];
	for (s = 1; s < size; ++s)
		if (_max < vec1[s])
			_max = vec1[s];

	return _max;
}

/* implementation of mul operator */
melem_t amin(const melem_t v1, const melem_t v2) {
	return 1.0 - tanh(atanh(1.0 - v1) + atanh(1.0 - v2));
}

/* implementation of plus operator */
melem_t amax(const mvec1_t vec1, const msize_t size) {
	melem_t v = 0;
	msize_t i;
	for (i = 0; i < size; ++i)
		v += atanh(vec1[i]);
		
	return tanh(v);
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
    atm->feat.size = feature_num;

	/* init range according to the split value */
	atm->range = (feat_t)_calloc(atm->splits + 1, sizeof(felem_t));

	if ((ret = automata_split_range(atm)))
		return ret;

	/* init fuzzy */
#ifdef FUZZY_TYPE
	atm->fuzzy = 1;
#else
	atm->fuzzy = 0;
#endif

	automata_init_matrix(atm);

	return ATM_OK;
}

void init_from_vec(double *vec, automata_t *atm) {
	msize_t split, n, k;
	double max, prev_max, nval;
	int i, j;
    int non_d_num = 1;
	
	if(!vec)
		return;
		
	/* for each split */
	for(split = 0; split < atm->mtx.m; ++split) {
        /* for each column */
        for (k = 0; k < atm->mtx.k; ++k) {
            max = prev_max = -DBL_MAX;
            i = j = -1;
            /* for each row */
            for (n = 0; n < atm->mtx.n; ++n) {
				nval = vec[split * (atm->sym_class_num * atm->sym_class_num) + atm->sym_class_num * n + k];
                atm->mtx.mtx[split][MTX_2D(n, k, atm->mtx.k)] = 0;
                if (max < nval) {
                    if(i != -1 && k < non_d_num) {
						prev_max = max;
						j = i;
					} 
                    max = nval;
                    i = n;
                } 
                else if(k < non_d_num && prev_max < nval) {
					prev_max = nval;
					j = n;
				}
            }
			
            if (i == -1) {
                fprintf(stderr, "Negative value\n");
                exit(EXIT_FAILURE);
            }
            atm->mtx.mtx[split][MTX_2D(i, k, atm->mtx.k)] = 1;
            if(k < non_d_num)
				atm->mtx.mtx[split][MTX_2D(j, k, atm->mtx.k)] = 1;
        }
    }
    
    /*for(split = 0; split < atm->mtx.m; ++split) {
		for (n = 0; n < atm->mtx.n; ++n) {
			for (k = 0; k < atm->mtx.k; ++k) {
                printf("%d ", atm->mtx.mtx[split][MTX_2D(n, k, atm->mtx.k)]);
			}
			printf("\n");
		}
		printf("\n");
	}
    
    getchar();*/
}

void init_from_dvec(double *vec, automata_t *atm) {
	msize_t split, n, k;
	double nval;
	
	if(!vec)
		return;
		
	/* for each split */
	for(split = 0; split < atm->mtx.m; ++split)
        /* for each column */
        for (k = 0; k < atm->mtx.k; ++k)
            /* for each row */
            for (n = 0; n < atm->mtx.n; ++n)
                atm->mtx.mtx[split][MTX_2D(n, k, atm->mtx.k)] = 
					vec[split * (atm->sym_class_num * atm->sym_class_num) + atm->sym_class_num * n + k];
}

void init_from_vec_old(double *vec, automata_t *atm) {
	msize_t split, n, k;
	double max;
	int i;
	
	if(!vec)
		return;
		
	/* for each split */
	for(split = 0; split < atm->mtx.m; ++split) {
        /* for each column */
        for (k = 0; k < atm->mtx.k; ++k) {
            max = -DBL_MAX;
            i = -1;
            /* for each row */
            for (n = 0; n < atm->mtx.n; ++n) {
                atm->mtx.mtx[split][MTX_2D(n, k, atm->mtx.k)] = 0;
                if (max < vec[split * (atm->sym_class_num * atm->sym_class_num) +
                              atm->sym_class_num * n + k]) {
								  
                    max = vec[split * (atm->sym_class_num * atm->sym_class_num) +
                          atm->sym_class_num * n + k];
                    i = n;
                }
            }
			
            if (i == -1) {
                fprintf(stderr, "Negative value\n");
                exit(EXIT_FAILURE);
            }
            atm->mtx.mtx[split][MTX_2D(i, k, atm->mtx.k)] = 1;
        }
    }
}

void automata_free(automata_t *atm) {
    msize_t i;

    free(atm->range);

    for(i = 0; i < atm->mtx.m; ++i)
        free(atm->mtx.mtx[i]);

    free(atm->mtx.mtx);
}

void automata_build(double *vec, automata_t *atm, msize_t input_size, feature_t *features, double *err_num) {
	atm_err_code ret;
    fsize_t s, q;
	mvec1_t out_vec,
			cs_vec; /* current state vector */
    msize_t i, vec_size, valid_num = 0, j;
    double tmp_err;
	
	if (atm->fuzzy)
		init_from_dvec(vec, atm);
	else init_from_vec(vec, atm);

	/* choose a start state here */
	/*matrix_set_cols(&(atm->mtx));*/
	vec_size = atm->mtx.k;

	atm->stat.errors = 0;
	atm->stat.whole = 0;
	atm->stat.fuzzy_errors = 0.0;

	cs_vec = (mvec1_t)_calloc(vec_size, sizeof(melem_t));

	for (i = 0; i < input_size; ++i) {
		/*printf("\rLetter %d/%d", i + 1, input_size);*/
		atm->feat = features[i];
		atm->state = SYM_A;
        memset(cs_vec, 0, vec_size * sizeof(melem_t));
		cs_vec[atm->state] = 1;

		/* for each element in deterministic split vector */
		for (s = 0; s < atm->feat.size; ++s) {
			if((ret = matrix_mul(&(atm->mtx), atm->feat.determin_splits[s], cs_vec, atm->mtx.k, &out_vec)))
				/*return ret;*/
				exit(EXIT_FAILURE);
			
			memcpy(cs_vec, out_vec, vec_size * sizeof(melem_t));

			free(out_vec);
        }

        /*if(!err_num) {
        for(s = 0; s < vec_size; ++s)
            printf("%d ", cs_vec[s]);

        printf("\n");
        }*/

        if(!err_num) {
            /*for(s = 0; s < vec_size; ++s) {
                if(cs_vec[s]) {
                    printf("Input %d = symbol %d\n", i, s);
                    valid_num++;
                    break;
                }
            }*/
        }

		/* check if out state equals to correct state, if no increase errors+ */
		++atm->stat.whole;
		if (atm->fuzzy) {
			tmp_err = 0.0;
			for (j = 0; j < vec_size; ++j)
				tmp_err += (atm->feat.correct == j) ? 
							abs(cs_vec[j] - 1.0) : cs_vec[j];
							
			atm->stat.fuzzy_errors += tmp_err;
			
		}
		else if(!cs_vec[atm->feat.correct])
			++atm->stat.errors;
	}

	/* printf("\nErrors %d\n", atm->stat.errors); */

	free(cs_vec);

    if(err_num) {
        if(atm->fuzzy) {
			*err_num = atm->stat.fuzzy_errors;
		}
		else *err_num = (double)atm->stat.errors;
	}
    else {
		printf("Total error:%f%%\n", atm->stat.fuzzy_errors);
		printf("Error: %f%%\n", 100.0 * ((!atm->fuzzy ? atm->stat.errors : atm->stat.fuzzy_errors) / (double)input_size));
	}

	/* FIXME: should be free */
	/*
	matrix_free(&(atm->mtx));
	free(atm->range);
	*/
	
	/*return ATM_OK;*/
	return;
}

/* starts automata building */
atm_err_code automata_build_start(automata_t *atm, msize_t input_size, feature_t *features, msize_t repeat) {
	atm_err_code ret;
	fsize_t s;
	mvec1_t out_vec,
			cs_vec; /* current state vector */
	msize_t i, j, vec_size;

	/* choose a start state here */
	matrix_set_cols(&(atm->mtx));
	vec_size = atm->mtx.k;

	for(j = 0; j < repeat; ++j)
	{
		atm->stat.errors = 0;
		atm->stat.whole = 0;

		printf("\nRound %d/1000\n", j + 1);
		cs_vec = (mvec1_t)_calloc(vec_size, sizeof(melem_t));

		for (i = 0; i < input_size; ++i) {
			printf("\rLetter %d/%d", i + 1, input_size);
			atm->feat = features[i];
			atm->state = SYM_A;
			cs_vec[atm->state] = 1;

			/* for each element in deterministic split vector */
			for (s = 0; s < atm->feat.size; ++s) {
				if((ret = matrix_mul(&(atm->mtx), atm->feat.determin_splits[s], 
									 cs_vec, atm->mtx.k, &out_vec)))
					return ret;
				
				memcpy(cs_vec, out_vec, vec_size * sizeof(melem_t));

				free(out_vec);
			 }

			/* check if out state equals to correct state, if no increase errors+ */
			++atm->stat.whole;
			if (!cs_vec[atm->feat.correct])
				++atm->stat.errors;
		}

		printf("\nErrors %d\n", atm->stat.errors);

		free(cs_vec);

		/* FIXME: call a PSO here with atm->stat.whole, atm->stat.error and atm->mtx */
		/* pso() */
	}

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
        atm->range[s] = (double)s / atm->splits;

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
	if (atm->fuzzy) { 
		if ((ret = matrix_set_add(&(atm->mtx), amax)))
			return ATM_MTX_INIT_ERR;
		if ((ret = matrix_set_mul(&(atm->mtx), amin)))
			return ATM_MTX_INIT_ERR;
	}
	else {
		if ((ret = matrix_set_add(&(atm->mtx), max)))
			return ATM_MTX_INIT_ERR;
		if ((ret = matrix_set_mul(&(atm->mtx), min)))
			return ATM_MTX_INIT_ERR;
	}

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
ftr_err_code automata_feature_normalize(automata_t *atm, feature_t *feat) {
	fsize_t s;
	msize_t k;

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
	if (!atm->range)
		return ATM_RANGE_IS_NULL;

	feat->determin_splits = (srdet_t)_calloc(feat->size, sizeof(fsize_t));
	for (s = 0; s < feat->size; ++s) {
		for(k = 0; k < atm->splits; ++k) {
			if(feat->feat[s] <= atm->range[k + 1]) {
				feat->determin_splits[s] = k;
				break;
			}
			if(k == atm->splits - 1)
				feat->determin_splits[s] = k;

		}
	}

	return FTR_OK;
}

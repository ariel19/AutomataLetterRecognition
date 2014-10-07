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

/* responsible for normalizing specified feature vector */
ftr_err_code normalize(feat_t feat, const feat_t normal, const fsize_t size) {
	return FTR_OK;
}

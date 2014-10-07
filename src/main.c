#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "matrix.h"
#include "alr.h"

int main(int argc, char **argv) {
	matrix_t mtx;
	melem_t vec1[] = { 0, 1, 0, 0 };
	mvec1_t vec;
	msize_t s;
	
	srand(time(NULL));
	
	puts("Hello, automata!");
	matrix_init(&mtx, 2, 3, 4);
	matrix_set_add(&mtx, max);
	matrix_set_mul(&mtx, min);
	
	matrix_show(&mtx, 0);
	matrix_show(&mtx, 1);
	
	matrix_set_cols(&mtx);
	
	matrix_show(&mtx, 0);
	matrix_show(&mtx, 1);
	
	matrix_mul(&mtx, 0, vec1, 4, &vec);
	
	for (s = 0; s < 3; ++s)
		fprintf(stdout, "%.1d ", vec[s]);
	fprintf(stdout, "\n");

	matrix_mul(&mtx, 1, vec1, 4, &vec);
	
	for (s = 0; s < 3; ++s)
		fprintf(stdout, "%.1d ", vec[s]);
	fprintf(stdout, "\n");
	
	free(vec);
		
	matrix_free(&mtx);
	
	return EXIT_SUCCESS;
}

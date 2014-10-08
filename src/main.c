#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "matrix.h"
#include "alr.h"

int main(int argc, char **argv) {
	msize_t splits_num = 5;
	msize_t symbol_class_num = 26;
	fsize_t feature_num = 10;
	msize_t input_size = 3000;
	msize_t i;
	
	automata_t atm;
	feat_t max; /* TODO: malloc & init */
	feature_t *features = (feature_t*)_calloc(input_size, sizeof(feature_t));
	
	/* TODO: init all vars from file */
	/* TODO: init features */
	/* TODO: init max */

	puts("Hello, automata!");
	
	srand(time(NULL));
	automata_init(&atm, &max, feature_num, splits_num, symbol_class_num);
	
	for(i = 0; i < input_size; ++i)
		automata_feature_normalize(&atm, &features[i]);
	
	automata_build_start(&atm, input_size, features);
	
	
	free(features);
		
	return EXIT_SUCCESS;
}

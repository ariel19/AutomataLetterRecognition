#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "matrix.h"
#include "alr.h"

int read_data(msize_t *splits_num, msize_t *symbol_class_num, fsize_t *feature_num, msize_t *input_size, msize_t *repeat,
				feat_t *max)
{
	unsigned int i;
	FILE *f = NULL;
	f = fopen("example.dat", "r");
	
	if(!f)
		return 1;
		
	fscanf(f, "%u, %u, %u, %u, %u, \n\n", (unsigned int*)splits_num, (unsigned int*)symbol_class_num, (unsigned int*)feature_num, 
		(unsigned int*)input_size, (unsigned int*)repeat);
		
	*max = (feat_t)_calloc(*feature_num, sizeof(felem_t));
		
	for(i = 0; i < *input_size; ++i)
		fscanf(f, "%lf, ", (double*)&(*max)[i]);
		
	fclose(f);
	
	return 0;
}

int main(int argc, char **argv) {
	msize_t splits_num = 0;
	msize_t symbol_class_num = 0;
	fsize_t feature_num = 0;
	msize_t input_size = 0;
	msize_t repeat = 0;
	msize_t i;
	
	automata_t atm;
	feat_t max; /* TODO: malloc & init */
	feature_t *features;
	
	if(read_data(&splits_num, &symbol_class_num, &feature_num, &input_size, &repeat, &max))
		return 1;
	
	features = (feature_t*)_calloc(input_size, sizeof(feature_t));
	
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

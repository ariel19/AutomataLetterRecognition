#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "matrix.h"
#include "alr.h"

int read_data(const char* filename, msize_t *splits_num, msize_t *symbol_class_num, 
				fsize_t *feature_num, msize_t *input_size, msize_t *repeat, msize_t *test_size,
				feat_t *max, feature_t **features, feature_t **test_features)
{
	unsigned int i, j;
	FILE *f = NULL;
	f = fopen(filename, "r");
	
	if(!f)
		return 1;
		
	fscanf(f, "%u, %u, %u, %u, %u, %u, \n\n", (unsigned int*)splits_num, (unsigned int*)symbol_class_num, (unsigned int*)feature_num, 
		(unsigned int*)input_size, (unsigned int*)repeat, (unsigned int*)test_size);
		
	*max = (feat_t)_calloc(*feature_num, sizeof(felem_t));
		
	for(i = 0; i < *feature_num; ++i)
		fscanf(f, "%lf, ", (double*)&(*max)[i]);
		
	fscanf(f, "\n\n");
		
	*features = (feature_t*)_calloc(*input_size, sizeof(feature_t));
	*test_features = (feature_t*)_calloc(*test_size, sizeof(feature_t));
	
	for(i = 0; i < *input_size; ++i) {
		(*features)[i].feat = (feat_t)_calloc(*feature_num, sizeof(felem_t));
		(*features)[i].size = *feature_num;
		
		fscanf(f, "%d\n", (int*)&((*features)[i].correct));
		
		for(j = 0; j < *feature_num; ++j)
			fscanf(f, "%lf, ", (double*)&((*features)[i].feat[j]));
			
		fscanf(f, "\n\n");
	}
	
	for(i = 0; i < *test_size; ++i) {
		(*test_features)[i].feat = (feat_t)_calloc(*feature_num, sizeof(felem_t));
		(*test_features)[i].size = *feature_num;
		
		for(j = 0; j < *feature_num; ++j)
			fscanf(f, "%lf, ", (double*)&((*test_features)[i].feat[j]));
			
		fscanf(f, "\n\n");
	}
		
	fclose(f);
	
	return 0;
}

int main(int argc, char **argv) {
	msize_t splits_num = 0;
	msize_t symbol_class_num = 0;
	fsize_t feature_num = 0;
	msize_t input_size = 0;
	msize_t repeat = 0;
	msize_t test_size = 0;
	msize_t i;
	
	automata_t atm;
	feat_t max;
	feature_t *features;
	feature_t *test_features;
	
	if(argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}
	
	if(read_data(argv[1], &splits_num, &symbol_class_num, &feature_num, &input_size, &repeat, &test_size, &max, &features, &test_features))
		return 1;

	puts("Hello, automata!");
	
	srand(time(NULL));
	automata_init(&atm, &max, feature_num, splits_num, symbol_class_num);
	
	for(i = 0; i < input_size; ++i)
		automata_feature_normalize(&atm, &features[i]);
	
	automata_build_start(&atm, input_size, features);
	
	free(features);
	free(test_features);
		
	return EXIT_SUCCESS;
}

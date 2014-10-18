#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "matrix.h"
#include "alr.h"
#include "swarm.h"

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
	msize_t error_num;
	
	unsigned int num_part = 10;
	double min_x = 0.0, max_x = 1.0;
	
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
	
	srand(time(NULL));
	automata_init(&atm, &max, feature_num, splits_num, symbol_class_num);
	
	for(i = 0; i < input_size; ++i)
		automata_feature_normalize(&atm, &features[i]);
	
	/* automata_build_start(&atm, input_size, features, repeat); */
	
	puts("Starting pso...");
	pso(atm.mtx.m * atm.mtx.n * atm.mtx.k,
		num_part,
		min_x,
		max_x,
		repeat,
		0.0,
		automata_build,
		&atm,
		input_size,
		features);
		 
	puts("PSO finished");
	puts("Running with test data...");
	
	for(i = 0; i < test_size; ++i)
		automata_feature_normalize(&atm, test_features[i]);
	
	automata_build(NULL, &atm, test_size, test_features, &error_num);
	
	printf("%u errors for test input.\n", error_num);

    automata_free(&atm);
	
	for(i = 0; i < input_size; ++i) {
		free(features[i].determin_splits);
		free(features[i].feat);
	}
		
	for(i = 0; i < test_size; ++i) {
		free(test_features[i].determin_splits);
		free(test_features[i].feat);
	}
	
	free(features);
	free(test_features);
	free(max);
		
	return EXIT_SUCCESS;
}

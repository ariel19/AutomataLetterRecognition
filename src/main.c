#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "matrix.h"
#include "alr.h"
#include "swarm.h"

int read_data(const char* filename, int *is_rej, msize_t *splits_num, msize_t *symbol_class_num,
              fsize_t *feature_num, msize_t *train_size, msize_t *test_size, double *max_los, double *min_los,
              feature_t **features, feature_t **test_features, pso_params_t *psopar)
{
    unsigned int i, j;
    FILE *f = NULL;
    f = fopen(filename, "r");

    if(!f)
        return 1;

#ifdef IS_REJECTED
    *is_rej = 1;
#else
    *is_rej = 0;
#endif

    fscanf(f, "%u, %u, %u, %u, %u, %lf, %lf, \n", (unsigned int*)symbol_class_num, (unsigned int*)feature_num,
           (unsigned int*)splits_num, (unsigned int*)train_size, (unsigned int*)test_size, min_los, max_los);

    fscanf(f, "%u, %u, \n\n", &psopar->iterations, &psopar->swarmsize);

    *features = (feature_t*)_calloc(*train_size, sizeof(feature_t));
    *test_features = (feature_t*)_calloc(*test_size, sizeof(feature_t));

    for(i = 0; i < *train_size; ++i) {
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

        fscanf(f, "%d\n", (int*)&((*test_features)[i].correct));

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
    msize_t train_size = 0;
    msize_t test_size = 0;
    msize_t i;
    pso_params_t psoparams;
    double max_los, min_los;
    int is_rej = 0;

    double min_x = 0.0, max_x = 1.0;

    automata_t atm;
    feature_t *features;
    feature_t *test_features;

    if(argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if(read_data(argv[1], &is_rej, &splits_num, &symbol_class_num,
                 &feature_num, &train_size, &test_size, &max_los, &min_los,
                 &features, &test_features, &psoparams))
        return 1;

    if(is_rej)
        symbol_class_num++;

    srand(time(NULL));
    automata_init(&atm, max_los, min_los, feature_num, splits_num, symbol_class_num);

    for(i = 0; i < train_size; ++i)
        automata_feature_normalize(&atm, &features[i]);

    puts("Starting pso...");
    pso(atm.mtx.m * atm.mtx.n * atm.mtx.k,
        psoparams.swarmsize,
        min_x,
        max_x,
        psoparams.iterations,
        /*0.0,*/
        automata_build,
        &atm,
        train_size,
        features);

    puts("PSO finished");
    puts("Running with test data...");

    for(i = 0; i < test_size; ++i)
        automata_feature_normalize(&atm, &test_features[i]);

    automata_build(NULL, &atm, train_size, features, NULL);
    automata_build(NULL, &atm, test_size, test_features, NULL);

    /*print_atm(&atm);*/
    // TODO: print features

    automata_free(&atm);

    for(i = 0; i < train_size; ++i) {
        free(features[i].determin_splits);
        free(features[i].feat);
    }

    for(i = 0; i < test_size; ++i) {
        free(test_features[i].determin_splits);
        free(test_features[i].feat);
    }

    free(features);
    free(test_features);

    return EXIT_SUCCESS;
}

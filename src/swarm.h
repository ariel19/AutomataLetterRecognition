/** @file PSO header */

#ifndef _SWARM_H
#define _SWARM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "types.h"
#include "alr.h"

typedef struct Particle
{
    double*  position;
    double   error;
    double*  velocity;
    double*  bestPosition;
    double   bestError;
} Particle;

/**
 * @brief PSO
 * @param dim size of the problem
 * @param numParticles number of particles used in the algorithm
 * @param minX minimal value of particles' position
 * @param maxX maximal value of particles' position
 * @param maxEpochs maximal number of iterations
 * @param exitError
 * @param errorFuncion the function calculating error of the current position
 *
 * @return solution vector
 */
double* pso_solve(unsigned int dim, unsigned int numParticles, double minX, double maxX,
                  unsigned int maxEpochs, double exitError, double (*errorFunction)(double *x, int dim));
                  
/**
 * @brief PSO
 * @param dim: Size of the problem
 * @param minX: Minimal value of particles' position
 * @param maxX: Maximal value of particles' position
 * @param atm: Automata
 * @param errorFunction: Error function
 * @param input_size: Size of input
 * @param features: Features
 * @param psoparams: PSO parameters
 * @param nondet_prop: Nondeterministic automata limit
 */
void pso(unsigned int dim, double minX, double maxX, /*double exitError, */
         void (*errorFunction)(double *, automata_t *, msize_t, feature_t *, double *, double),
         automata_t *atm, msize_t input_size, feature_t *features, pso_params_t *psoparams, double nondet_prop);

#endif

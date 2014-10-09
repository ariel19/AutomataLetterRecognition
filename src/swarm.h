#ifndef _SWARM_H
#define _SWARM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "types.h"

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
 * @param dim dimension of smth
 * @param
 * @param
 * @param
 * @param
 * @param
 * @param
 *
 * @return solution vector
 */
double* pso_solve(unsigned int dim, unsigned int numParticles, double minX, double maxX,
                  unsigned int maxEpochs, double exitError, double (*errorFunction)(double *x, int dim));

#endif

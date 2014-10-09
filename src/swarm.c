#include "swarm.h"
#include "syswrap.h"
#include <float.h>

/* PSO */
double* pso_solve(unsigned int dim, unsigned int numParticles, double minX, double maxX,
              unsigned int maxEpochs, double exitError, double (*errorFunction)(double *x, int dim)) {

    /* srand(time(NULL)); */

    Particle* swarm = (Particle*)malloc(numParticles * sizeof(Particle));
    double*   bestGlobalPosition = (double*)malloc(dim *sizeof(double));
    double    bestGlobalError = DBL_MAX;
    double error;
    double lo, hi;

    unsigned int i, j, k;
    double* randomPosition;
    double* randomVelocity;

    double w;         /* inertia weight */
    double c1;        /* cognitive weight */
    double c2;        /* social weight */
    double r1, r2;    /* cognitive and social randomizations */
    double probDeath;

    unsigned int epoch;

    double* newPosition;
    double* newVelocity;
    double  newError;

    /* Initialization */
    for(i=0; i<numParticles; ++i){
        randomPosition = (double*)_malloc(dim *sizeof(double));
        randomVelocity = (double*)_malloc(dim *sizeof(double));

        for(j=0; j< dim; ++j)
            randomPosition[j] = (maxX-minX)*( (double)rand()/RAND_MAX ) + minX;

        error = errorFunction(randomPosition, dim);

        for(j=0; j<dim;++j){
            lo = minX * 0.1;
            hi = maxX * 0.1;
            randomVelocity[j] =  (hi-lo)*( (double)rand()/RAND_MAX ) + lo;
        }
        swarm[i].position     = randomPosition;
        swarm[i].velocity     = randomVelocity;
        swarm[i].error        = error;
        swarm[i].bestPosition = randomPosition;
        swarm[i].bestError    = error;

        if(swarm[i].error<bestGlobalError){
            bestGlobalError = swarm[i].error;

            /* Copy particles position to bestGlobalPosition */
            for(j=0;j<dim;++j){
                bestGlobalPosition[j] = swarm[i].position[j];
            }

        }
    } /* Initialization */

    w = 0.729;
    c1 = 1.49445;
    c2 = 1.49445;
    probDeath = 0.01;

    epoch = 0;

    newPosition = (double*)_malloc(dim *sizeof(double));
    newVelocity = (double*)_malloc(dim *sizeof(double));

    /* main loop */
    while(epoch<maxEpochs){
        for(i=0; i< numParticles; ++i){

            Particle currP = swarm[i];

            /* find new velocity */
            for(j = 0; j<numParticles; ++j){
                r1 = (double)rand()/RAND_MAX;
                r2 = (double)rand()/RAND_MAX;

                newVelocity[j] = (w * currP.velocity[j]) +
                                 (c1 * r1 * (currP.bestPosition[j] - currP.position[j])) +
                                 (c2 * r2 * (bestGlobalPosition[j] - currP.position[j]));
                /* Change current particle velocity to newVelocity */
                for( k=0; k<dim;++k)
                    currP.velocity[k] = newVelocity[k];

            }
            /* find new position according to new velocity */
            for(j=0;j<dim;++j){
                newPosition[j] = currP.position[j] + newVelocity[j];
                if(newPosition[j] < minX)
                    newPosition[j] = minX;
                /* else if() */
            }
        }
    }
    return NULL;
}

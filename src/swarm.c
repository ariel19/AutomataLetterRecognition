#include "swarm.h"
#include "syswrap.h"
#include <float.h>

/* PSO */
double* pso_solve(unsigned int dim, unsigned int numParticles, double minX, double maxX,
              unsigned int maxEpochs, double exitError, double (*errorFunction)(double *x, int dim)) {


    Particle* swarm = (Particle*)_malloc(numParticles * sizeof(Particle));
    double*   bestGlobalPosition = (double*)_malloc(dim * sizeof(double));
    double    bestGlobalError = DBL_MAX;
    double error;
    double lo, hi;

    unsigned int i, j, k; /* indicies */
    double* randomPosition;
    double* randomVelocity;

    double w;         /* inertia weight */
    double c1;        /* cognitive weight */
    double c2;        /* social weight */
    double r1, r2;    /* cognitive and social randomizations */
    double probDeath;
    double die;

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
            }
            /* Change current particle velocity to newVelocity */
            for( k=0; k<dim;++k)
                currP.velocity[k] = newVelocity[k];
            /* find new position according to new velocity */
            for(j=0;j<dim;++j){
                newPosition[j] = currP.position[j] + newVelocity[j];
                if(newPosition[j] < minX)
                    newPosition[j] = minX;
                else if(newPosition[j] > maxX)
                    newPosition[j] = maxX;
            }
            /* set new position */
            for( k = 0; k<dim;++k)
                currP.position[k] = newPosition[k];

            newError = errorFunction(newPosition, dim);
            currP.error = newError;

            if (newError < currP.bestError){
                for( k=0; k<dim;++k)
                    currP.bestPosition[k] = newPosition[k];
                currP.bestError = newError;
            }
            /* set best global error */
            if (newError < bestGlobalError)
            {
                for( k=0; k<dim;++k)
                    bestGlobalPosition[k] = newPosition[k];
                bestGlobalError = newError;
            }
            /* Death */
            die = (double)rand()/RAND_MAX;

            if (die < probDeath)
            {
                for (j = 0; j < dim; ++j)
                    currP.position[j] = (maxX-minX)*( (double)rand()/RAND_MAX ) + minX;
                currP.error = errorFunction(currP.position, dim);

                for(k=0; k<dim; ++k)
                    currP.bestPosition[k]=currP.position[k];

                currP.bestError = currP.error;

                if (currP.error < bestGlobalError)
                {
                    bestGlobalError = currP.error;
                    for(k=0; k<dim; ++k)
                        bestGlobalPosition[k]=currP.position[k];
                }
            }
        } /* each particle */
        ++epoch;
    }

    /* deinitialization */
    for(i=0;i<dim;++i){
        free(swarm[i].position);
        free(swarm[i].velocity);
        free(swarm[i].bestPosition);

    }
    free(swarm);
    free(newPosition);
    free(newVelocity);

    return bestGlobalPosition;
}

void pso(unsigned int dim, unsigned int numParticles, double minX, double maxX, unsigned int maxEpochs, double exitError, 
		 void (*errorFunction)(double *, automata_t *, msize_t, feature_t *, msize_t *),
		 automata_t *atm, msize_t input_size, feature_t *features) {
			 
	Particle* swarm = (Particle*)_malloc(numParticles * sizeof(Particle));
    double*   bestGlobalPosition = (double*)_malloc(dim *sizeof(double));
    double    bestGlobalError = DBL_MAX;
    unsigned int uerror;
    double error;
    double lo, hi;
    msize_t mi, mj;

    unsigned int i, j, k; /* indicies */
    double* randomPosition;
    double* randomVelocity;

    double w;         /* inertia weight */
    double c1;        /* cognitive weight */
    double c2;        /* social weight */
    double r1, r2;    /* cognitive and social randomizations */
    double probDeath;
    double die;

    unsigned int epoch;

    double* newPosition;
    double* newVelocity;
    double  newError;

    /* Initialization */
    for(i=0; i<numParticles; ++i){
        randomPosition = (double*)_calloc(dim, sizeof(double));
        randomVelocity = (double*)_calloc(dim, sizeof(double));
		
		/*
        for(j=0; j< dim; ++j)
            randomPosition[j] = (maxX-minX)*( (double)rand()/RAND_MAX ) + minX;
		*/
		
		/* init automata with 0, 1 */
		for (mi = 0; mi < atm->splits; ++mi) {
			/* init columns */
			for (mj = 0; mj < atm->sym_class_num; ++mj) {
				randomPosition[mi * (atm->sym_class_num * atm->sym_class_num) + 
							   mj * atm->sym_class_num + rand() % atm->sym_class_num] = 1;
			}
		}
		
        /* error = errorFunction(randomPosition, dim); */
        errorFunction(randomPosition, atm, input_size, features, &uerror);
        error = uerror;

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
            }
            /* Change current particle velocity to newVelocity */
            for( k=0; k<dim;++k)
                currP.velocity[k] = newVelocity[k];
            /* find new position according to new velocity */
            for(j=0;j<dim;++j){
                newPosition[j] = currP.position[j] + newVelocity[j];
                if(newPosition[j] < minX)
                    newPosition[j] = minX;
                else if(newPosition[j] > maxX)
                    newPosition[j] = maxX;
            }
            /* set new position */
            for( k = 0; k<dim;++k)
                currP.position[k] = newPosition[k];

            /*newError = errorFunction(newPosition, dim);*/
			errorFunction(newPosition, atm, input_size, features, &uerror);
			newError = uerror;
            currP.error = newError;

            if (newError < currP.bestError){
                for( k=0; k<dim;++k)
                    currP.bestPosition[k] = newPosition[k];
                currP.bestError = newError;
            }
            /* set best global error */
            if (newError < bestGlobalError)
            {
                for( k=0; k<dim;++k)
                    bestGlobalPosition[k] = newPosition[k];
                bestGlobalError = newError;
            }
            /* Death */
            die = (double)rand()/RAND_MAX;

            if (die < probDeath)
            {
                for (j = 0; j < dim; ++j)
                    currP.position[j] = (maxX-minX)*( (double)rand()/RAND_MAX ) + minX;
                
                /* currP.error = errorFunction(currP.position, dim);*/
				errorFunction(currP.position, atm, input_size, features, &uerror);
				currP.error = uerror;

                for(k=0; k<dim; ++k)
                    currP.bestPosition[k]=currP.position[k];

                currP.bestError = currP.error;

                if (currP.error < bestGlobalError)
                {
                    bestGlobalError = currP.error;
                    for(k=0; k<dim; ++k)
                        bestGlobalPosition[k]=currP.position[k];
                }
            }
        } /* each particle */
        ++epoch;
    }

    /* deinitialization */
    for(i=0;i<dim;++i){
        free(swarm[i].position);
        free(swarm[i].velocity);
        free(swarm[i].bestPosition);

    }
    free(swarm);
    free(newPosition);
    free(newVelocity);

	/* TODO: init atm matrix here */

	init_from_vec(bestGlobalPosition, atm);
	free(bestGlobalPosition); 
}

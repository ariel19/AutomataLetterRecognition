#include <math.h>

#include "psoTest.h"
#include "swarm.h"

double doubleDipError(double* x, int dim)
{
  double trueMin = -0.42888194;
  double z = x[0] *
    exp( -((x[0]*x[0]) + (x[1]*x[1])) );
  return (z - trueMin) * (z - trueMin);
}
void psoTest()
{
    double exactSolution[2] = {-0.7071068, 0.0};
    int epochs = 1000;
    int numParticles = 10;

    double* p = pso_solve(2, numParticles, -10, 10, epochs, 0.0, doubleDipError);

    printf("================= PSO TEST =================\n\n");
    printf("Find minimum of z = x * exp( -(x^2 + y^2) )\n");
    printf("Epochs = %d\n", epochs);
    printf("Number of particles = %d\n", numParticles);
    printf("Pso solution = (%f, %f)\nPrecise value = (%f, %f)\nError = %f\n\n",p[0], p[1], exactSolution[0], exactSolution[1], doubleDipError(p,2) );
    printf("============================================\n");
}

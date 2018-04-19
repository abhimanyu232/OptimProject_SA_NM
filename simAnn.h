#include "optimisation.h"

#define TOL 1e-16
#define T0  100

bool cooling(int choice, double temp);
int simAnnealing(int dim, double (*foo)(int, const VectorXd&));

#include "optimisation.h"

// set initial temperature //
#define T0  100

bool cooling(int choice, double temp);
int simAnnealing(int dim, double (*fitness)(int, const VectorXd&));

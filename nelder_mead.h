#include "optimisation.h"

#define TOL 1e-16
#define T0  100

int nelderMead(int dim, double (*foo)(int, const MatrixXd&));

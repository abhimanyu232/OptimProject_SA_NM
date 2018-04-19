#include "optimisation.h"

#define T0  100

int nelderMead(int dim, double (*fitness)(int, const MatrixXd&));

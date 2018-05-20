#include "optimisation.h"

void sort_simplex(const int& dim, VectorXd& fitness, MatrixXd& simplex);
void shrink_simplex(const int& dim, int& shrink, MatrixXd& simplex);
void find_barycentre(const int& dim, VectorXd& mean, MatrixXd simplex);

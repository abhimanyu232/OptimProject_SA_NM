#include<iostream>
#include<iomanip>
#include<fstream>
#include<math.h>
#include<Eigen/Core>
#include<random>
#include<time.h>
#include<assert.h>

#define DIM_MAX 20
#define TOL 1e-16
#define ITER_MAX 1e10

using namespace Eigen;
using namespace std;

typedef double (*fitVXd)(int, const VectorXd&);
typedef double (*fitMXd)(int, const MatrixXd&);
// fitness functions overloaded for different implementations
// one each for Nelder Mead and Simulated Annealing
// necessary definitions in respective .cpp files.
// driver.cpp MUST BE COMPILED with either nelder_mead.cpp or simAnn.cpp
int testFCN_choice(fitVXd& fcnPtr);
int testFCN_choice(fitMXd& fcnPtr);

double rosenbrock_2d(int dim, const VectorXd& X); // for simAnn
double rosenbrock_2d(int dim, const MatrixXd& X); // for nelderMead

double rosenbrock_Nd(int dim ,const VectorXd& X); // for simAnn
double rosenbrock_Nd(int dim ,const MatrixXd& X); // for nelderMead

double sphere_Nd(int dim,const VectorXd& X ); // for simAnn
double sphere_Nd(int dim,const MatrixXd& X ); // for nelderMead

double egghol(int dim, const VectorXd& X); // for simAnn
double egghol(int dim, const MatrixXd& X); // for nelderMead

int simAnnealing(int testfcn, int dim, double(*fitness)(int, const VectorXd&));
int nelderMead(int testfcn, int dim, double(*fitness)(int, const MatrixXd&));

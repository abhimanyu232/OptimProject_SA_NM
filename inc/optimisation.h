#include<iostream>
#include<iomanip>
#include<fstream>
#include<math.h>
#include<Eigen/Core>
#include<random>
#include<time.h>
#include<assert.h>

// mkdir headers //
#include <string.h>
#include <limits.h>     // PATH_MAX
#include <sys/stat.h>   // nmkdir(2)
#include <errno.h>
// //


#define DIM_MAX 20
#define TOL 1e-16
#define ITER_MAX 1e9

using namespace Eigen;
using namespace std;

typedef double (*fitVXd)(int, const VectorXd&);
typedef double (*fitMXd)(int, const MatrixXd&);

int testFCN_choice(fitVXd& fcnPtr);
int testFCN_choice(fitMXd& fcnPtr);

int simAnnealing(int testfcn, int dim, fitVXd);
int nelderMead(int testfcn, int dim, fitMXd);

// fitness functions overloaded for different implementations
// one each for Nelder Mead and Simulated Annealing
// necessary definitions in respective .cpp files.
// driver.cpp MUST BE COMPILED with either nelder_mead.cpp or simAnn.cpp
double rosenbrock_2d(int dim, const VectorXd& X); // for simAnn
double rosenbrock_2d(int dim, const MatrixXd& X); // for nelderMead

double rosenbrock_Nd(int dim ,const VectorXd& X); // for simAnn
double rosenbrock_Nd(int dim ,const MatrixXd& X); // for nelderMead

double sphere_Nd(int dim,const VectorXd& X ); // for simAnn
double sphere_Nd(int dim,const MatrixXd& X ); // for nelderMead

double egghol(int dim, const VectorXd& X); // for simAnn
double egghol(int dim, const MatrixXd& X); // for nelderMead

double schaf(int dim, const VectorXd& X);
//double schaf(int dim, const MatrixXd& X);

int mkdir_p(const char *path);

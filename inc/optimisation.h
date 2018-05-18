#include<iostream>
#include<iomanip>
#include<fstream>
#include<math.h>
#include<Eigen/Core>
#include<random>
#include<assert.h>


// mkdir headers //
#include <string.h>
#include <limits.h>     // PATH_MAX
#include <sys/stat.h>   // nmkdir(2)
#include <errno.h>
#define DIM_MAX 20
#define TOL 1e-16

using namespace Eigen;
using namespace std;

int mkdir_p(const char *path);

typedef double (*fitVXd)(const int&, const VectorXd&);
//typedef double (*fitMXd)(int, const MatrixXd&);

int simAnnealing(const int& testfcn,const int& dim, fitVXd foo);
int nelderMead(const int& testfcn,const int& dim, fitVXd foo);

int testFCN_choice(fitVXd& fcnPtr);
//int testFCN_choice(fitMXd& fcnPtr);

double rosenbrock_2d(const int& dim, const VectorXd& X);
//double rosenbrock_2d(int dim, const MatrixXd& X);

double rosenbrock_Nd(const int& dim,const VectorXd& X);
//double rosenbrock_Nd(int dim ,const MatrixXd& X);

double sphere_Nd(const int& dim,const VectorXd& X );
//double sphere_Nd(int dim,const MatrixXd& X );

double egghol(const int& dim, const VectorXd& X);
//double egghol(int dim, const MatrixXd& X);

double schaf(const int& dim, const VectorXd& X);
//double schaf(int dim, const MatrixXd& X);

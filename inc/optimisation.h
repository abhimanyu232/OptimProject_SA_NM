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
#define DIM_MAX 50
#define TOL 1e-16

using namespace Eigen;
using namespace std;

// pointer to function that takes (const int&,const VectorXd&) arguements
typedef double (*fitVXd)(const int&, const VectorXd&);

int mkdir_p(const char *path);

int simAnnealing(const int& testfcn,const int& bounds,const int& dim, fitVXd foo);
int nelderMead(const int& testfcn,const int& bounds,const int& dim, fitVXd foo);

double rosenbrock_2d(const int& dim, const VectorXd& X);
double rosenbrock_Nd(const int& dim,const VectorXd& X);
double sphere_Nd(const int& dim,const VectorXd& X );
double egghol(const int& dim, const VectorXd& X);
double schaf(const int& dim, const VectorXd& X);

int testFCN_choice(fitVXd& fcnPtr);
int domain_limit(const int& testfcn);
void enforce_boundary(const int& dim,const int& bounds, VectorXd& next);

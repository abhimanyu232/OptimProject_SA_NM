#include<iostream>
#include<Eigen/Core>
#include<math.h>
#include<assert.h>

#define DIM_MAX 10


using namespace Eigen;
using namespace std;

//template <typename Derived> // use Vector2d
double rosenbrock_2d(int dim, const VectorXd& X);

//template <typename Derived> // use VectorXd
double rosenbrock_Nd(int dim ,const VectorXd& X);

//template <typename Derived>// use VectorNd
double sphere_Nd(int dim,const VectorXd& X );

//template <typename Derived>// use Vector2d
double egghol(int dim, const VectorXd& X);

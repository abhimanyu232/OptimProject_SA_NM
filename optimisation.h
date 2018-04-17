#include<iostream>
#include<Eigen/Core>
#include<math.h>

using namespace Eigen;
using namespace std;

template <typename Derived> // use Vector2d
double  rosenbrock_2d(double a, double b, const MatrixBase<Derived>& X);

template <typename Derived> // use VectorXd
double rosenbrock_Nd(int dim,double a,double b,const MatrixBase<Derived>& X);

template <typename Derived>// use VectorNd
double sphere_Nd(int dim,const MatrixBase<Derived>& X);

template <typename Derived>// use Vector2d
double egghol(const MatrixBase<Derived>& X);

#include"test_functions.hpp"

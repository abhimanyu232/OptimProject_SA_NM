#include<iostream>
#include<Eigen/Core>
#include<math.h>

using namespace Eigen;
using namespace std;

template <typename Derived>
double rosenbrock_2d(double, double, const MatrixBase<Derived>& ); // use Vector2d
double rosenbrock_Nd(double, double, const MatrixBase<Derived>& ); // use VectorXd
double sphere_3d(const MatrixBase<Derived>&); // Vector3d
double egghol(const MatrixBase<Derived>&); //use Vector2d

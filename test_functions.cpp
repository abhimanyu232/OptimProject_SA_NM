//#include"test_functions.h"
#include<iostream>
#include<Eigen/Dense>
#include<math.h>

using namespace Eigen;
using namespace std;

template <typename Derived>
long rosenbrock_2d(double a, double b, const MatrixBase<Derived>& X  ){

double r2d =  pow(a - X(0),2) + b*pow(X(1) - X(0),2);

  return r2d;
}


int main(){

Vector2d v1;
v1 << 1,1;
v1 << Vector2d::Random();
cout <<"x :" << v1(0) << "y :" << v1(1)<< endl;
double fit = rosenbrock_2d(1,100,v1);
cout << "fitness = " << fit << endl;

  return 0;
}

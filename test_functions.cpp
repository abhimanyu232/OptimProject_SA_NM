//#include"test_functions.h"

#include<iostream>
#include<Eigen/Dense>
#include<math.h>

using namespace Eigen;
using namespace std;

template <typename Derived>
double  rosenbrock_2d(double a, double b, const MatrixBase<Derived>& X  ){
double r2d =  pow(a - X(0),2) + b*pow(X(1) - X(0),2);

  return r2d;
}

template <typename Derived>
double rosenbrock_Nd(int dim, double, double, const MatrixBase<Derived>& X){ // use VectorXd

}

template <typename Derived>
double sphere_3d(int dim,const MatrixBase<Derived>& X){ // VectorNd
double sph3D = 0;
for (int i =0; i<=dim ; i++)
 sph3D = sph3D + pow(X(i),2);

return sph3D;
}

template <typename Derived>
double egghol(const MatrixBase<Derived>&){ //use Vector2d
double eggh;


return eggh;
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

#include"optimisation.h"
#include"nelder_mead.h"


int nelderMead(int dim, double (*foo)(int, const MatrixXd& fitness)){
// use fitness as a black box fitness function. takes arguement of type MatrixXd.
// declaration in nelder_mead.h
// add all function/variables declarations that are needed only
// for nelder_mead in nelder_mead.h
// everything that needs to be accessed by driver.cpp needs to be declared in
// optimisation.h

}



//~~~~~~~~~~~~~~~~~~~~~Function Definitions~~~~~~~~~~~~~~~~~~`//`

// 2D Rosenbrock Function
// global minima f(x,y) = 0 at (x,y) = (a,a*a)
//
//template <typename Derived> // use Vector2d
double  rosenbrock_2d(int dim, const MatrixXd& X){
  assert(dim==2);
  double r2d =  pow(1 - X(0),2) + 100*pow(X(1) - X(0),2);
return r2d;
}

// Multidimensional Rosenbrock Function
// global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
//
//template <typename Derived> // use VectorXd
double rosenbrock_Nd(int dim ,const MatrixXd& X){
  double rNd =0.;
  for (int i=0; i<dim-1;i++)
      rNd = rNd + ( 100*(pow(X(i+1) - pow(X(i),2),2)) + pow(1-X(i),2) );
return rNd;
}

// Multi Dimensional Sphere Function
// global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
//
//template <typename Derived>// use VectorNd
double sphere_Nd(int dim,const MatrixXd& X ){
  double sph = 0;
  for (int i =0; i<dim ; i++)
      sph = sph + pow(X(i),2);
return sph;
}

//2D Eggholder Function
//  global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
//
//template <typename Derived>// use Vector2d
double egghol(int dim, const MatrixXd& X){
  assert(dim == 2);
  double eggh = -1*(X(1)+47)*sin(sqrt(fabs(X(0)+(X(1)+47))))
                            - X(0)*sin(sqrt(fabs(X(0)-(X(1)+47))));
return eggh;
}

// Schaffer Function : has a plateau
// Cross in Tray or Himmebau : multiple global minimas
// dejong5 fcn

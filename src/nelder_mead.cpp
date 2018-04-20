#include"optimisation.h"
#include"nelder_mead.h"


int nelderMead(int testfcn, int dim, double (*fit)(int, const MatrixXd&)){
// use fit as a black box fitness function.it takes arguement of type MatrixXd.
// where it points to is decided by user // switch statement from driver.cpp

// add any function/variables declarations that are needed only for the nelder
//mead method in nelder_mead.h // add corresponding declarations here //
// everything that needs to be accessed by driver.cpp needs to be declared in
// optimisation.h and defined in driver.cpp

// imo the points could be stored in matrix form with dim+1 rows and dim cols.
// it could be implemented as MatrixXd points(dim+1,dim)
// each row stores 1 point coordinates in dim dimensional space
// but feel free to use another method if you like

return 0;
}



//~~~~~~~~~~~~~~~~~~~~~Function Definitions~~~~~~~~~~~~~~~~~~`//`

// 2D Rosenbrock Function
// global minima f(x,y) = 0 at (x,y) = (a,a*a)
double  rosenbrock_2d(int dim, const MatrixXd& X){
  assert(dim==2);
  double r2d =  pow(1 - X(0),2) + 100*pow(X(1) - X(0),2);
return r2d;
}

// Multidimensional Rosenbrock Function
// global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
double rosenbrock_Nd(int dim ,const MatrixXd& X){
  double rNd =0.;
  for (int i=0; i<dim-1;i++)
      rNd = rNd + ( 100*(pow(X(i+1) - pow(X(i),2),2)) + pow(1-X(i),2) );
return rNd;
}

// Multi Dimensional Sphere Function
// global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
double sphere_Nd(int dim,const MatrixXd& X ){
  double sph = 0;
  for (int i =0; i<dim ; i++)
      sph = sph + pow(X(i),2);
return sph;
}

//2D Eggholder Function
//  global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
double egghol(int dim, const MatrixXd& X){
  assert(dim == 2);
  double eggh = -1*(X(1)+47)*sin(sqrt(fabs(X(0)+(X(1)+47))))
                            - X(0)*sin(sqrt(fabs(X(0)-(X(1)+47))));
return eggh;
}

// Schaffer Function : has a plateau
// Cross in Tray or Himmebau : multiple global minimas
// dejong5 fcn

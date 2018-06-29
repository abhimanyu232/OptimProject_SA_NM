#include"optimisation.h"

//~~~~~~~~~~~Test Function Definitions~~~~~~~~~~~~~~~~~~~~//
// 2D Rosenbrock Function
// global minima f(x,y) = 0 at (x,y) = (a,a*a)
double  rastrigin(const int& dim, const VectorXd& X){
  double rstgnND = 10*dim ;
  for (int i = 0; i<dim ; i++)
      rstgnND = rstgnND + ( pow(X(i),2) - 10*cos(2*M_PI*X(i)) ) ;

return rstgnND;
}

// Multidimensional Rosenbrock Function
// global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
double rosenbrock_Nd(const int& dim,const VectorXd& X){
  double rNd =0.;
  for (int i=0; i<dim-1;i++)
      rNd = rNd + ( 100*(pow(X(i+1) - pow(X(i),2),2)) + pow(1-X(i),2) );
return rNd;
}

//~~~~~~~~~~~Test Function Choice~~~~~~~~~~~~~~~~~~~~//

void P_testFCN_choice(const int& choice,fitVXd& fitness){
switch (choice) {
  case 1:
    fitness = rastrigin;
    //std::cout << "rastrigin chosen" << '\n';
    break;

  case 2:
    fitness = rosenbrock_Nd;
    //std::cout << "rosenbrock_Nd chosen" << '\n';
    break;
}
}

// ------------AUXILLARY FUNCTIONS-------------- //

int domain_limit(const int& testfcn){
  int boundary;
  if (testfcn == 4)         // eggholder
    boundary = 512;
  else if (testfcn == 1)    // Rastrigin
    boundary = 5.12;
  else
    boundary = 1000;        // Usual

return boundary;
}


void enforce_boundary(const int& dim,const int& bounds, VectorXd& next){
  for (int i = 0 ; i<dim ; i++){
      if (fabs(next(i))<=bounds){
  return;
  }
    else {
          if (fabs(next(0))>bounds){
            for (int i = 0 ; i<dim ; i++) {
                if (next(i)<0) {
                  next(i) = -1*bounds + fabs(next(i)+bounds);
                }
                else if (next(i)>=0) {
                  next(i) =  bounds - fabs(next(i)-bounds);
                }
            }
        }
    }
  }
return;
}


// simulated annealing //
/*
VectorXd = Matrix<double, dim, 1>
RowVectorXd = Matrix<double,1,dim>
RowVectorXd curr, new, best_pt

1.  Temperature = 100 ; curr = random() ; best_pt = curr;
do {
2. new = curr  + delta //
3. if fit(new) <fit(curr)
        curr  = new
        if fit(new) < fit(best)
        best = curr
else if fit(new) > fit(curr) && p(k,Temp) > rand()
           curr =  new
          }
4. Temp = Temp*whatever [cooling scheme]
  }
5. // REANNEALING??
*/

#include "optimisation.h"
#include "simAnn.h"

// DECIDE IF YOU WANT TO PASS THE VECTOR AS A POINTER OR CARRY OUT
// EVERYTHING INSDE THE simAnnealing FUNCTION WITH DRIVER JUST CALLING IT

// typedef double (*fitPtr)(int, const VectorXd&);
int simAnnealing(int dim, double (*fit)(int, const VectorXd& fitness) ){
  double temp;
  int choice = 0;

  cout << "select cooling scheme:\n"
  "1: temp = T0*pow(0.95,k)\n "
  "2: temp = T0/k\n"
  "3: temp = T0/log(k)\n";
  while( !(std::cin >> choice;) || (choice < 1) || (choice > 3) ){
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::cout << "please enter a valid choice\n "
    "1: temp = T0*pow(0.95,k)\n "
    "2: temp = T0/k\n"
    "3: temp = T0/log(k)\n";
  }


return 0;
}


// k annealing parameter // same as iteration until reannealing //
bool cooling(const int choice, int k, double *temp){
  switch (choice) {
  case 1: *temp = T0*pow(0.95,k); return 1;

  case 2: *temp = T0/k; return 1;

  case 3: *temp = T0/log(k); return 1;

  default: cout << "invalid cooling choice" << '\n'; return 0;
}
}

//~~~~~~~~~~~Test Function Definitions~~~~~~~~~~~~~~~~~~~~//


// 2D Rosenbrock Function
// global minima f(x,y) = 0 at (x,y) = (a,a*a)
//
//template <typename Derived> // use Vector2d
double  rosenbrock_2d(int dim, const VectorXd& X){
  assert(dim==2);
  double r2d =  pow(1 - X(0),2) + 100*pow(X(1) - X(0),2);
return r2d;
}

// Multidimensional Rosenbrock Function
// global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
//
//template <typename Derived> // use VectorXd
double rosenbrock_Nd(int dim ,const VectorXd& X){
  double rNd =0.;
  for (int i=0; i<dim-1;i++)
      rNd = rNd + ( 100*(pow(X(i+1) - pow(X(i),2),2)) + pow(1-X(i),2) );
return rNd;
}

// Multi Dimensional Sphere Function
// global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
//
//template <typename Derived>// use VectorNd
double sphere_Nd(int dim,const VectorXd& X ){
  double sph = 0;
  for (int i =0; i<dim ; i++)
      sph = sph + pow(X(i),2);
return sph;
}

//2D Eggholder Function
//  global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
//
//template <typename Derived>// use Vector2d
double egghol(int dim, const VectorXd& X){
  assert(dim == 2);
  double eggh = -1*(X(1)+47)*sin(sqrt(fabs(X(0)+(X(1)+47))))
                            - X(0)*sin(sqrt(fabs(X(0)-(X(1)+47))));
return eggh;
}

// Schaffer Function : has a plateau
// Cross in Tray or Himmebau : multiple global minimas
// dejong5 fcn

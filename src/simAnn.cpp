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


int simAnnealing(int testfcn, int dim, double (*fit)(int, const VectorXd&) ){
  // random number seeding //
  std::random_device rd;
  std::mt19937 gen(rd());

  double temp;
  // CHOICE OF COOLING SCHEME //
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

  cout << "Iteration\tFitness Value\tBest Value" << '\n';

  Eigen::VectorXd curr,best,next;
  curr = VectorXd::Random(dim)*1000; // random points b/w (-1000,1000)
  best = curr;

  double fit_curr,fit_next,fit_best;
  fit_curr = fit(curr);
  fit_best = fit_curr;

  temp = T0;
  int iter = 0;
  int k,acnt;
  double residual = 1.;
  do {
    iter++;
    k=iter;
    //k=iter-(reanneal_step*100); // changes after reannealing //
    //
    // insert step ensure domain boundedness and correct //
    //
      next = curr + VectorXd::Random(dim)*MAX_STEP; // can implement differently
      fit_next=fit(next);
      residual = fit_curr - fit_next;

      std::bernoulii_dustribution Pb(PAccept(temp,fit_curr,fit_next));
      if (fit_next<fit_curr) {    // good point
          curr = next;
          fit_curr = fit_next;
          if (fit_next<fit_best) {
              best = next;
              fit_best=fit_next;
          }
      }
      else if (fit_next>=fit_curr) { // bad point
            if (Pb(gen)) {
              curr = next;
              fit_curr=fit_next;
            }
      }

      /* ??? REANNEAL ???
      whole loop surrounding calculations
      for (accept_count=0;accept_count<100;){
      increment accept_count only if accept i.e in first part of if loop above.
      reset k 1
      reset temperature
      }
      */
      cooling(choice, k , temp);

      if ((iter%REPORT_INTERVAL)==0){
          cout << setw(9) <<iter<<"\t"<<setprecision(6)<<setw(13)<<fit_curr<<
          "\t"<<setprecision(6)<<setw(10)<<fit_best<<'\n';
      }

  }
  while ( iter <= ITER_MAX || residual >= TOL  );


return 0;
}


// k annealing parameter // same as iteration until reannealing //
// initial temperature set in simAnn.h //
bool cooling(const int choice, int k, double *temp){
  switch (choice) {
  case 1: *temp = T0*pow(0.95,k); return 1;

  case 2: *temp = T0/k; return 1;

  case 3: *temp = T0/log(k); return 1;

  default: cout << "invalid cooling choice" << '\n'; return 0;
}
}

// prob of accepting bad value //
float PAccept(double temp, double fit_curr, fit_next){
return exp((-1)*(fabs(fit_curr-fit_next))/temp);
}

//~~~~~~~~~~~Test Function Definitions~~~~~~~~~~~~~~~~~~~~//
// 2D Rosenbrock Function
// global minima f(x,y) = 0 at (x,y) = (a,a*a)
double  rosenbrock_2d(int dim, const VectorXd& X){
  assert(dim==2);
  std::cout << X << '\n';
  double r2d =  1.;
return r2d;
}

// Multidimensional Rosenbrock Function
// global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
double rosenbrock_Nd(int dim ,const VectorXd& X){
  double rNd =0.;
  std::cout << X << '\n';
return rNd;
}

// Multi Dimensional Sphere Function
// global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
double sphere_Nd(int dim,const VectorXd& X ){
  double sph = 0;
  for (int i =0; i<dim ; i++)
      sph = sph + pow(X(i),2);
return sph;
}

//2D Eggholder Function
//  global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
double egghol(int dim, const VectorXd& X){
  assert(dim == 2);
  assert((X(0)>=-512) && (X(1)<=512));
  double eggh = -1*(X(1)+47)*sin(sqrt(fabs(X(0)+(X(1)+47))))
                      - X(0)*sin(sqrt(fabs(X(0)-(X(1)+47))));
return eggh;
}

// Schaffer Function : has a plateau
// Cross in Tray or Himmebau : multiple global minimas
// dejong5 fcn

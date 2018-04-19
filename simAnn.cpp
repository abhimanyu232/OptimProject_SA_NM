
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
int simAnnealing( double (*fit)(int, const VectorXd&) ){
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

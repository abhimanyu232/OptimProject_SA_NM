#include "optimisation.h"
#include "simAnn.h"


int simAnnealing(int testfcn, int dim, fitVXd fit){
  // random number seeding //
  std::random_device rd;
  std::mt19937 gen(rd());

  // CHOICE OF COOLING SCHEME //
  int coolScheme = 0;
  cooling_choice(&coolScheme);  // sets variable coolScheme

  // initialise search space //
  Eigen::VectorXd curr,next,best;
  if (testfcn == 4)
    curr = VectorXd::Random(2)*512;
  else
    curr = VectorXd::Random(dim)*SEARCH_DOMAIN; // random start

  best = curr;
  //std::cout << best  << '\n';

  double fit_curr,fit_next,fit_best;
  fit_curr = fit(dim,curr);
  fit_best = fit_curr;
  //std::cout << "/* test message */" << '\n';
  cout << "Iteration\tFitness Value\tBest Value\tTemperature\t"
  "Accept Count\tRe-Anneal Count" << endl;

  int k=0;        // annealing parameter
  int reAnnCnt = 0;
  int iter=0;
  int acnt=0;     // counts number of accepted values of next
  double residual=1.;
  double temp=T0;
  do {
    do {
      iter++;
      k++;
      next = curr + (VectorXd::Random(dim)*temp).
      cwiseProduct((round(ArrayXd::Random(dim))).matrix());

      // bound check for eggholder function
      if (testfcn == 4){
          if (fabs(next(0))<=512.0 || fabs(next(1))<=512.0){
                continue;
          }
          else {
              if (fabs(next(0))>512){
                  if (next(0)<0) {
                      next(0) = -512 + fabs(next(0)+512);
                  }
                  else if (next(0)>=0) {
                      next(0) =  512 - fabs(next(0)-512);
                  }
              }
              if (fabs(next(1))>512){
                  if (next(1)<0) {
                      next(1) = -512 + fabs(next(1)+512);  /* code */
                  }
                  if (next(1)>=0) {
                      next(1) = 512 - fabs(next(1)-512);
                  }
              }
          }
      }

      fit_next= fit(dim,next);
      residual = fabs(fit_curr - fit_next);

      std::bernoulli_distribution Pb(PAccept(temp,fit_curr,fit_next));
      if (fit_next<fit_curr) {    // good point
          curr = next;
          fit_curr = fit_next;
          acnt++;                 // acceptance count increment
          if (fit_next<fit_best) {
              best = next;
              fit_best=fit_next;
          }
      }
      else if (fit_next>=fit_curr) { // bad point
            if (Pb(gen)) {
              curr = next;
              fit_curr=fit_next;
              acnt++;
            }
      }
      cooling(coolScheme, k , &temp);

      if (iter%1000000==0) {
        cout << "Iteration\tFitness Value\tBest Value\tTemperature\t"
        "N.Accept \tN.Re-Anneal" << endl;
      }

      if ((iter%REPORT_INTERVAL)==0){
          cout << setw(9) <<iter<<"\t"<<setprecision(10)<<setw(13)<<fit_curr<<
          "\t"<<setprecision(6)<<setw(10)<<fit_best<<'\t'<<setw(10)<<
          temp<<'\t'<<acnt<<'\t'<<reAnnCnt<<endl;
      }
    } while( acnt <=100 );
      reAnnCnt++;
      k = 1;
      temp = T0*0.95;
      acnt = 0;
      curr = best;
      fit_curr = fit_best;

/*
      if ((iter%REPORT_INTERVAL)==0){
          cout << setw(9) <<iter<<"\t"<<setprecision(10)<<setw(13)<<fit_curr<<
          "\t"<<setprecision(6)<<setw(10)<<fit_best<<'\t'<<setw(10)<<
          temp<<'\t'<<acnt<<'\t'<<reAnnCnt<<endl;
      }
*/
  }
  while ( iter <= ITER_MAX );
//std::cout << "residual" << residual<< '\n';
return 0;
}

void cooling_choice(int * choice){
  cout << "select cooling scheme:\n"
  "1: temp = T0*pow(0.95,k)\n "
  "2: temp = T0/k\n"
  "3: temp = T0/log(k) **RECOMMENDED** \n";
  while( !(std::cin >> (*choice)) || ((*choice) < 1) || ((*choice) > 3) ){
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::cout << "please enter a valid choice\n "
    "1: temp = T0*pow(0.95,k)\n "
    "2: temp = T0/k\n"
    "3: temp = T0/log(k) **RECOMMENDED**\n ";
  }
}

// k annealing parameter // same as iteration until reannealing //
// initial temperature set in simAnn.h //
bool cooling(const int choice, int k, double *temp){
  switch (choice) {
  case 1: *temp = T0*pow(0.95,k); return 1;  // BEST

  case 2: *temp = T0/k; return 1;       //fast annealing

  case 3: *temp = T0/log(k); return 1; //boltzman annealing

  default: cout << "invalid cooling choice" << '\n'; return 0;
}
}

// prob of accepting bad value //
float PAccept(double temp, double fit_curr, double fit_next){
//return exp( (-1)*(fabs(fit_curr-fit_next))/(K_BOLTZ*temp) );
return 1/(1+exp((fit_next-fit_curr)/temp));
}

//~~~~~~~~~~~Test Function Definitions~~~~~~~~~~~~~~~~~~~~//
// 2D Rosenbrock Function
// global minima f(x,y) = 0 at (x,y) = (a,a*a)
double  rosenbrock_2d(int dim, const VectorXd& X){
  assert(dim==2);
  double r2d =  pow(1 - X(0),2) + 100*pow(X(1) - X(0),2);
return r2d;
}

// Multidimensional Rosenbrock Function
// global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
double rosenbrock_Nd(int dim ,const VectorXd& X){
  double rNd =0.;
  for (int i=0; i<dim-1;i++)
      rNd = rNd + ( 100*(pow(X(i+1) - pow(X(i),2),2)) + pow(1-X(i),2) );
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
  assert(fabs(X(0))<=512);
  assert(fabs(X(1))<=512);
  std::cout << "chosen vector" << X << '\n';
  double eggh = -1*(X(1)+47)*sin(sqrt(fabs((X(0)*0.5)+(X(1)+47))))
                      - X(0)*sin(sqrt(fabs(X(0)-(X(1)+47))));
return eggh;
}

double schaf(int dim,const VectorXd& X){
  assert(dim==2);
  double sch = 0.5 + (pow(cos(pow(sin(fabs(X(0)*X(0)-X(1)*X(1))),2)),2) -0.5)/
    pow( (1+0.001*((X(0)*X(0)) + (X(1)*X(1)))), 2 );
return sch;
}

// Cross in Tray or Himmebau : multiple global minimas
// dejong5 fcn


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

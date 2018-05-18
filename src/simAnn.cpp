#include "optimisation.h"
#include "simAnn.h"
#include"get_time.h"

extern double ITER_MAX;
extern double REPORT_INTERVAL;
int simAnnealing(const int& testfcn,const int& dim, fitVXd fit){

  // CHOICE OF COOLING SCHEME //
  int coolScheme = 0;
  cooling_choice(&coolScheme);  // sets variable coolScheme

  ofstream result_file("results/Sim_Ann.dat", ios::app);
  if ( result_file.is_open() ){
    result_file << "Iteration \t Fitness Value " << endl ;
    result_file.close();
  } else {printf("ERROR OPENING DAT FILE\n");}

  int64 time_begin,time_end;
  time_begin=GetTimeMs64();

  // random number seeding //
  std::random_device rd;
  std::mt19937 gen(rd());

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
  cout << "Iteration\tFitness Value\t  Best Value\tTemperature\t"
  "Accept Count\tRe-Anneal Count" << endl;

  int k=0;        // annealing parameter
  int reAnnCnt = 0;
  int iter=0;
  int acnt=0;     // counts number of accepted values of next
  //double residual=1.;
  double temp=T0;

  result_file.open("results/Sim_Ann.dat", ios::app);
  if (!result_file.is_open()){
    std::cout << "unable to write data to file" << '\n';
    return 0;
  }
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
      //residual = fabs(fit_curr - fit_next);

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

      // WRITE TO FILE
      if ((fmod(iter,REPORT_INTERVAL))==0){
            result_file << setw(9) <<iter<<"\t"<<
            setprecision(10)<<setw(13)<<fit_curr<< endl ;
      }

      // PRINT SCREEN
      if ( fmod(iter,(REPORT_INTERVAL*10000)) ==0 ) {
        cout << "Iteration\tFitness Value\t  Best Value\tTemperature\t"
        "N.Accept \tN.Re-Anneal" << endl;
      }
      if ( fmod(iter,(REPORT_INTERVAL*100)) ==0 ){
          cout << setw(9) <<iter<<"\t"<<setprecision(6)<<setw(13)<<fit_curr<<
          "\t  "<<setprecision(6)<<setw(10)<<fit_best<<'\t'<<setw(8)<<
          setprecision(6)<<temp<<'\t'<<setw(12)<<acnt<<'\t'<<reAnnCnt<<endl;
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
  time_end=GetTimeMs64();
  std::cout <<'\n' << "BEST POINT:"<< '\n';
  std::cout << best << '\n';
  std::cout << "Time Elapsed: " << time_end - time_begin << "ms" << '\n';
  result_file.close();
//std::cout << "residual" << residual<< '\n';
return 0;
}

void cooling_choice(int * choice){
  cout << "select cooling scheme:\n"
  "1: temp = T0*pow(0.95,k) **SUPER RECOMMENDED** \n "
  "2: temp = T0/k\n *Quite Trash*"
  "3: temp = T0/log(k) **SLOW RECOMMENDED** \n";
  while( !(std::cin >> (*choice)) || ((*choice) < 1) || ((*choice) > 3) ){
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::cout << "please enter a valid choice\n "
    "1: temp = T0*pow(0.95,k) **SUPER RECOMMENDED** \n "
    "2: temp = T0/k\n *Quite Trash*"
    "3: temp = T0/log(k) **SLOW RECOMMENDED** \n";
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

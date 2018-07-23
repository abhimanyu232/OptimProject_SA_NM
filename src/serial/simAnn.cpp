#include"optimisation.h"
#include"simAnn.h"

extern double ITER_MAX;
extern double REPORT_INTERVAL;

int simAnnealing(const int& testfcn,const int& bounds,const int& dim, fitVXd fit){
  double alpha = 0.95;
  double beta = 0.2;
  ofstream result_file("results/serial/SAnn_time.dat",ios::app);
  // CHOICE OF COOLING SCHEME //
  for (alpha=0.65;alpha<=.90;alpha+=0.05){
  result_file << "alpha = " << alpha << endl;
  int coolScheme = 0;
  for (coolScheme =1;coolScheme<=1;coolScheme++){
    result_file << "coolScheme:" << coolScheme <<endl;
  for (int i=0;i<20;i++){

  //cooling_choice(&coolScheme);  // sets variable coolScheme
  //ofstream result_file("results/serial/Sim_Ann.dat");
  //if ( result_file.is_open() ){
  //  result_file << "Iteration \t Fitness Value " << endl ;
  //  result_file.close();
  //} else {cerr<<"ERROR OPENING DAT FILE\n";}


  // BEGIN TIME //
  auto time_begin = Clock::now();

  // random number seeding //
  std::random_device rd;
  std::mt19937 gen(rd());

  // initialise search space //
  Eigen::VectorXd curr,next,best;
  curr = VectorXd::Random(dim)*bounds; // random start
  // BETTER INITIAL POINT USING OTHER ALGORITHM : NM GRADIENT SEARCH ETC //
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

  /*result_file.open("results/Sim_Ann.dat", ios::app);
  if (!result_file.is_open()){
    cerr << "unable to write data to file" << '\n';
    return 0;
  }
  */
  do {
    do {
      iter++;
      k++;
      next = curr + (VectorXd::Random(dim)*temp).
      cwiseProduct((round(ArrayXd::Random(dim))).matrix());
      //  BETTER RANDOM NUMBER GENERATOR //

      // enforce domain and reflect of out of domain
      enforce_boundary(dim,bounds,next);

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
      // MOVE COOLING OUTSIDE // I.E RUN 100 ITER AT SAME TEMPERATURE
      cooling(coolScheme, k , &temp,alpha,beta);

      // WRITE TO FILE
      /*if ((fmod(iter,REPORT_INTERVAL))==0){
        if (result_file){
            result_file << setw(9) <<iter<<"\t"<<
            setprecision(10)<<setw(13)<<fit_curr<< endl ;
        } else {cerr << "ERROR WRITING DATA TO FILE";}
      }*/

      // PRINT SCREEN
      if ( fmod(iter,(REPORT_INTERVAL*1000000)) ==0 ) {
        cout << "Iteration\tFitness Value\t  Best Value\tTemperature\t"
        "N.Accept \tN.Re-Anneal" << endl;
      }
      if ( fmod(iter,(REPORT_INTERVAL*1000)) ==0 ){
          cout << setw(9) <<iter<<"\t"<<setprecision(6)<<setw(13)<<fit_curr<<
          "\t  "<<setprecision(6)<<setw(10)<<fit_best<<'\t'<<setw(8)<<
          setprecision(6)<<temp<<'\t'<<setw(12)<<acnt<<'\t'<<reAnnCnt<<endl;
      }

    } while( acnt <=100  );
    // REANNEAL AND RESET PARAMETERS //
    reAnnCnt++;
    k = 1;
    temp = T0*0.95;
    acnt = 0;
    curr = best;
    fit_curr = fit_best;
  }
  while ( iter <= 50000000  && fit_best > 0.1 );

  // END TIME
  auto time_end = Clock::now();
  std::chrono::duration<double> time_elapsed = time_end-time_begin;
  std::cout <<'\n' << "BEST POINT:"<< '\n';
  std::cout << best << '\n';
  std::cout << "Time Elapsed: " << time_elapsed.count() << "ms" << '\n';
  if ( result_file.is_open() ){
    result_file << time_elapsed.count()  << endl;
  } else {cerr<<"ERROR OPENING DAT FILE\n";}
  //std::cout << "residual" << residual<< '\n';
  // 10 iterations
      }
    }
  } //coolScheme loop

return 0;
}

void cooling_choice(int * choice){
  cout << "select cooling scheme:\n"
  "1: temp = T0*pow(0.95,k) **SUPER RECOMMENDED** \n "
  "2: temp = T0/k\n "
  "3: temp = T0/log(k) **SLOW RECOMMENDED** \n";
  while( !(std::cin >> (*choice)) || ((*choice) < 1) || ((*choice) > 3) ){
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::cout << "please enter a valid choice\n "
    "1: temp = T0*pow(0.95,k) **SUPER RECOMMENDED** \n "
    "2: temp = T0/k\n "
    "3: temp = T0/log(k) **SLOW RECOMMENDED** \n";
  }
}

// k annealing parameter // same as iteration until reannealing //
// initial temperature set in simAnn.h //
bool cooling(const int choice, int k, double *temp, double alpha, double beta){
  switch (choice) {
  case 1: *temp = T0*pow(alpha,k); return 1;  // BEST

  case 2: *temp = T0/(beta*k); return 1;       //fast annealing

  case 3: *temp = T0/log(k); return 1; //boltzman annealing

  default: cout << "invalid cooling choice" << '\n'; return 0;
  }
}

// prob of accepting bad value //
float PAccept(double temp, double fit_curr, double fit_next){
//return exp( (-1)*(fabs(fit_curr-fit_next))/(K_BOLTZ*temp) );
return 1/(1+exp((fit_next-fit_curr)/temp));
}

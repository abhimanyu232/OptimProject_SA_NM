#include"optimisation.h"
#include"simAnn.h"
#include"get_time.h"

extern double ITER_MAX;
extern double REPORT_INTERVAL;

int simAnnealing(const int& testfcn,const int& bounds,const int& dim, fitVXd fit){
  // CHOICE OF COOLING SCHEME //
  int coolScheme = 0;
  cooling_choice(&coolScheme);  // sets variable coolScheme

  ofstream result_file("results/Sim_Ann.dat");
  if ( result_file.is_open() ){
    result_file << "Iteration \t Fitness Value " << endl ;
    result_file.close();
  } else {cerr<<"ERROR OPENING DAT FILE\n";}

  cout << "Iteration\tFitness Value\t  Best Value\tTemperature\t"
  "Accept Count\tRe-Anneal Count" << endl;


  int64 time_begin,time_end;
  // BEGIN TIME //

  // random number seeding //
  std::random_device rd;
  std::mt19937 gen(rd());

// -----------------BEGIN PARALLEL---------------- //

  struct {
    double best;
    int rank;
  }local_fit,global_fit;
  int proc_with_best_ = 0;

  Eigen::VectorXd LOCAL_curr,LOCAL_next,LOCAL_best;
  double LOCAL_fit_curr,LOCAL_fit_next,LOCAL_fit_best;

  time_begin=GetTimeMs64();

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  LOCAL_curr = VectorXd::Random(dim)*bounds; // random start
  LOCAL_fit_curr = fit(dim,LOCAL_curr);
  LOCAL_best = LOCAL_curr;
  LOCAL_fit_best = LOCAL_fit_curr;

  int k=0;        // annealing parameter
  int reAnnCnt = 0;
  int iter=0;
  int acnt=0;     // counts number of accepted values of next
  //double residual=1.;
  double temp=T0;

  do {  // ALL PROCESSES
      do {
// ------------------------ SEQUENTIAL SIMANN ------------------------- //
        //if (id) { // all processes but P0 //
          iter++;
          k++;
// GENERATE RANDOM STEP FOR EACH PROCESSOR //
          LOCAL_next = LOCAL_curr + (VectorXd::Random(dim)*(temp/T0)*bounds).
                       cwiseProduct((round(ArrayXd::Random(dim))).matrix()) +
                       bounds*(id/(2*size));
// enforce domain and reflect of out of domain
          enforce_boundary(dim,bounds,LOCAL_next);

          LOCAL_fit_next = fit(dim,LOCAL_next);
          //residual = fabs(fit_curr - fit_next);
          std::bernoulli_distribution Pb(PAccept(temp,LOCAL_fit_curr,LOCAL_fit_next));
          if (LOCAL_fit_next<LOCAL_fit_curr) {    // good point
            LOCAL_curr = LOCAL_next;
            LOCAL_fit_curr = LOCAL_fit_next;
            acnt++;                 // acceptance count increment
            if (LOCAL_fit_next<LOCAL_fit_best) {
                LOCAL_best = LOCAL_next;
                LOCAL_fit_best=LOCAL_fit_next;
            }
          }
          else if (LOCAL_fit_next>=LOCAL_fit_curr) { // bad point
            if (Pb(gen)) {
                LOCAL_curr = LOCAL_next;
                LOCAL_fit_curr = LOCAL_fit_next;
                acnt++;
            }
          }
// MOVE COOLING OUTSIDE // I.E RUN 100 ITER AT SAME TEMPERATURE
          cooling(coolScheme, k , &temp);
// ------------------------ SEQUENTIAL SIMANN END ------------------------- //
          local_fit.best = LOCAL_fit_best;
          local_fit.rank = id;
        //}
        MPI_Reduce(&local_fit,&global_fit,1,MPI_DOUBLE_INT,MPI_MINLOC,0,MPI_COMM_WORLD);
        MPI_Bcast(&global_fit,1,MPI_Double,0,MPI_COMM_WORLD);
// BROADCAST BEST POINT FROM BEST PROC TO EVERYONE ELSE AND REPEAT
        proc_with_best_ = global_fit.rank;
        MPI_Bcast(LOCAL_curr,dim,MPI_Double,proc_with_best_,MPI_COMM_WORLD);
//        if (id==0){ // Processor 0
//          proc_with_best_ = global_fit.rank;
//        }
//        MPI_Bcast(LOCAL_curr,dim,MPI_DOUBLE,proc_with_best_,MPI_COMM_WORLD)
        //LOCAL_curr = LOCAL_best;
      } while(anct <= 100);

        // REANNEAL AND RESET PARAMETERS //
        LOCAL_reAnnCnt++;
        LOCAL_k = 1;
        temp = T0*0.95;
        acnt = 0;
        LOCAL_curr = LOCAL_best;
        LOCAL_fit_curr = LOCAL_fit_best;

  } while (iter < ITER_MAX && global_fit.best > 0.1 )
// ----------------END PARALLEL----------------- //
  MPI_Finalize();
// END TIME
  time_end=GetTimeMs64();
return 0;
}

/*
  result_file.open("results/Sim_Ann.dat", ios::app);
  if (!result_file.is_open()){
    cerr << "unable to write data to file" << '\n';
    return 0;
  }
*/

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

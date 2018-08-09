#include"optimisation.h"
#include"simAnn.h"
#include<mpi.h>


double ITER_MAX = 10000;
double REPORT_INTERVAL = 50;

int main (int argc, char* argv[]){

    MPI_Init(&argc,&argv);
    int size, id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc == 1){
    if (!id)
    std::cout << "default ITER_MAX:"<< ITER_MAX << '\n';;
  }
  else if (argc >= 3){
    ITER_MAX = strtod(argv[1],NULL);
    REPORT_INTERVAL = strtod(argv[2],NULL);
    if (!id)
    std::cout << "ITER_MAX: " << ITER_MAX << "\tREPORT INTERVAL :" << REPORT_INTERVAL<< '\n';
  }

// TAKE COMMAND LINE ARGUEMENT TO REDUCE COMMUNICATION
    int dim = 15;
    /*if (id == 0){
      cout << "enter search space dimension ( 2<= Dim <="<<DIM_MAX<<")"<<endl;
      while (!(cin >> dim) || dim < 2 || dim > DIM_MAX)
      cout<<" value out of range (2<=Dimension<="<< DIM_MAX << ")"<< '\n';
    }
    MPI_Bcast(&dim,1,MPI_INT,0,MPI_COMM_WORLD);
    */
    if (dim==0)
    MPI_Finalize();
    srand((unsigned int) time(0));
    fitVXd fit = NULL;        // function pointer for fitness function
    // change function to take testfcn as arguement to remove the user input // 1 : Rastrigin // 2 : Rosenbrock
    int testfcn = 2;
    P_testFCN_choice(testfcn,fit);      // choice of test function
    int bounds = domain_limit(testfcn);     // set domain bounds
// CHOICE OF COOLING SCHEME //
    const int coolScheme = 1; // 1 , 2 or 3 //
    double alpha = 0.99; // .99 for Rosenbrock
    double beta = 0.2;
//  - --------------- MAIN ALGO - ----------------------------- //
      ofstream result_file("results/parallel/PSA_time.dat",ios::app);
      /*string path ="results/parallel/Sim_Ann_"+  to_string(id) + ".dat";
      ofstream result_file(path);
      if ( result_file.is_open() ){
        result_file << "Iteration \t Fitness Value " << endl ;
      } else {cerr<<"ERROR OPENING DAT FILE\n";}*/
      //cout << "Iteration\tFitness Value\t  Best Value\tTemperature\t"
      //"Accept Count\tRe-Anneal Count" << endl;

      // random number seeding //
      std::random_device rd;
      std::mt19937 gen(rd());

      struct {
        double best;
        int rank;
      }local_fit,global_fit;
      int proc_with_best_ = 0;

      Eigen::VectorXd lcl_curr,lcl_next,lcl_best;
      double lcl_curr_fit,lcl_next_fit,lcl_best_fit;

      MPI_Barrier(MPI_COMM_WORLD);
      // BEGIN TIME //
      auto time_begin = Clock::now();

      lcl_curr = VectorXd::Random(dim)*bounds; // random start
      lcl_curr_fit = fit(dim,lcl_curr);
      lcl_best = lcl_curr;
      lcl_best_fit = lcl_curr_fit;

      int k=0;        // annealing parameter
      int reAnnCnt = 0;
      int iter=0;
      int lcl_acnt=0;
      //int glbl_acnt=0;     // counts number of accepted values of best
      //double residual=1.;
      double temp=T0;
      double GLOBAL_MINIMUM = 1e12;

      if (!id) // ONLY FOR PROC 0 TO TRACK //
      GLOBAL_MINIMUM = lcl_best_fit ;

      do {  // ALL PROCESSES
          do {
    // ------------------------ SEQUENTIAL SIMANN ------------------------- //
            //if (id) { // all processes but P0 //
              iter++;
              k++;
    // GENERATE RANDOM STEP FOR EACH PROCESSOR //
              lcl_next = lcl_curr + (VectorXd::Random(dim)*(temp/T0)*bounds).
                           cwiseProduct((round(ArrayXd::Random(dim))).matrix()) +
                           MatrixXd::Ones(dim,1)*bounds*(id/(2*size));
    // enforce domain and reflect of out of domain
              enforce_boundary(dim,bounds,lcl_next);

              lcl_next_fit = fit(dim,lcl_next);
              //residual = fabs(fit_curr - fit_next);
              std::bernoulli_distribution Pb(PAccept(temp,lcl_curr_fit,lcl_next_fit));
              if (lcl_next_fit<lcl_curr_fit) {    // good point
                lcl_curr = lcl_next;
                lcl_curr_fit = lcl_next_fit;
                lcl_acnt++;                 // acceptance count increment
                if (lcl_next_fit<lcl_best_fit) {
                    lcl_best = lcl_next;
                    lcl_best_fit=lcl_next_fit;
                }
              }
              else if (lcl_next_fit>=lcl_curr_fit) { // bad point
                if (Pb(gen)) {
                    lcl_curr = lcl_next;
                    lcl_curr_fit = lcl_next_fit;
                    lcl_acnt++;
                }
              }
// MOVE COOLING OUTSIDE // I.E RUN 100 ITER AT SAME TEMPERATURE
              cooling(coolScheme, k , &temp,alpha,beta);
// ------------------------ SEQUENTIAL SIMANN END ------------------------- //
              local_fit.best = lcl_curr_fit;
              local_fit.rank = id;
            //}
            MPI_Allreduce(&local_fit,&global_fit,1,MPI_DOUBLE_INT,MPI_MINLOC,MPI_COMM_WORLD);
            //MPI_Allreduce(&lcl_acnt,&glbl_acnt,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
            //MPI_Bcast(&global_fit,1,MPI_Double,0,MPI_COMM_WORLD);
// BROADCAST BEST POINT FROM BEST PROC TO EVERYONE ELSE AND REPEAT
            proc_with_best_ = global_fit.rank;
            MPI_Bcast(lcl_curr.data(),lcl_curr.size(),MPI_DOUBLE,proc_with_best_,MPI_COMM_WORLD);

            if (!id){
              if (global_fit.best < GLOBAL_MINIMUM)
                  GLOBAL_MINIMUM = global_fit.best;
              //cout << "best fit "<< GLOBAL_MINIMUM << endl;
            }
            /*
            if (fmod(iter,REPORT_INTERVAL)==0){
              result_file << setw(9) <<iter<<"\t"<<
              setprecision(10)<<setw(13)<<local_fit.best<< endl ;
            }*/
          } while(lcl_acnt <= 100 && global_fit.best > 0.1);

            // REANNEAL AND RESET PARAMETERS //
            reAnnCnt++;
            k = 1;
            temp = T0*0.95;
            lcl_acnt = 0;
            lcl_curr = lcl_best;
            lcl_curr_fit = lcl_best_fit;
// iter < ITER_MAX &&
      } while ( global_fit.best > 0.1 );
// ----------------END PARALLEL----------------- //
// END TIME
      MPI_Barrier(MPI_COMM_WORLD);
      auto time_end = Clock::now();
      std::chrono::duration<double> time_elapsed = time_end-time_begin;
      if (!id){
        cout << "\nbest fit: "<< global_fit.best << endl;
        std::cout << "Time Elapsed: " << time_elapsed.count() << '\n';
        result_file << time_elapsed.count() << '\n';
      }
      MPI_Finalize();
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

#include"optimisation.h"
#include"simAnn.h"
#include"get_time.h"
#include<mpi.h>


extern double ITER_MAX;
extern double REPORT_INTERVAL;

int main (int argc, char* argv[]){

  int opt_choice;
  if (argc == 1){
    std::cout << "default ITER_MAX:"<< ITER_MAX << '\n';;
  }
  else if (argc == 3){
    ITER_MAX = strtod(argv[1],NULL);
    REPORT_INTERVAL = strtod(argv[2],NULL);
    std::cout << "ITER_MAX: " << ITER_MAX << "\tREPORT INTERVAL :" << REPORT_INTERVAL<< '\n';
  }
  else if (argc >= 4){
    opt_choice = atoi(argv[3]);
  }
  if (opt_choice != 1 && opt_choice != 2 ) {
    cout << "Choose optimisation method \n"
    "1: Nelder Mead  \n2: Simulated Annealing" << endl;
    while (!(cin >> opt_choice) || (opt_choice !=1 && opt_choice != 2)){
      cout << " Invalid Choice \n"
      "1: Nelder Mead \n2: Simulated Annealing" << endl;
    }
  }

  MPI_Init(&argc,&argv);
  int size, id;
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

// TAKE COMMAND LINE ARGUEMENT TO REDUCE COMMUNICATION
    int dim = 10;
    /*
    if (id == 0){
      // enter problem dimension //
      cout << "enter search space dimension ( 2<= Dim <="<<DIM_MAX<<")"<<endl;
      while (!(cin >> dim) || dim < 2 || dim > DIM_MAX)
      cout<<" value out of range (2<=Dimension<="<< DIM_MAX << ")"<< '\n';
    }
    MPI_Bcast(dim,1,MPI_INT,0,MPI_COMM_WORLD);
    */


    srand((unsigned int) time(0));
    fitVXd fit = NULL;        // function pointer for fitness function
    // change function to take testfcn as arguement to remove the user input
    int testfcn = testFCN_choice(fit);      // choice of test function
    int bounds = domain_limit(testfcn);     // set domain bounds

// CHOICE OF COOLING SCHEME //
// 1 , 2 or 3 //
    const int coolScheme = 1;

//  - --------------- MAIN ALGO - ----------------------------- //
      if (!id){
        ofstream result_file("results/Sim_Ann.dat");
        if ( result_file.is_open() ){
          result_file << "Iteration \t Fitness Value " << endl ;
          result_file.close();
        } else {cerr<<"ERROR OPENING DAT FILE\n";}

        cout << "Iteration\tFitness Value\t  Best Value\tTemperature\t"
        "Accept Count\tRe-Anneal Count" << endl;
      }


      int64 time_begin,time_end;
      // BEGIN TIME //

      // random number seeding //
      std::random_device rd;
      std::mt19937 gen(rd());

      struct {
        double next;
        int rank;
      }local_fit,global_fit;
      int proc_with_best_ = 0;

      Eigen::VectorXd LOCAL_curr,LOCAL_next,LOCAL_best;
      double LOCAL_fit_curr,LOCAL_fit_next,LOCAL_fit_best;

      MPI_Barrier(MPI_COMM_WORLD);
      time_begin=GetTimeMs64();  // chrono::highresolution maybe?

      LOCAL_curr = VectorXd::Random(dim)*bounds; // random start
      LOCAL_fit_curr = fit(dim,LOCAL_curr);
      LOCAL_best = LOCAL_curr;
      LOCAL_fit_best = LOCAL_fit_curr;

      int k=0;        // annealing parameter
      int reAnnCnt = 0;
      int iter=0;
      int LOCAL_acnt,GLOBAL_acnt=0;     // counts number of accepted values of next
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
                           MatrixXd::Ones(dim,1)*bounds*(id/(2*size));
    // enforce domain and reflect of out of domain
              enforce_boundary(dim,bounds,LOCAL_next);

              LOCAL_fit_next = fit(dim,LOCAL_next);
              //residual = fabs(fit_curr - fit_next);
              std::bernoulli_distribution Pb(PAccept(temp,LOCAL_fit_curr,LOCAL_fit_next));
              if (LOCAL_fit_next<LOCAL_fit_curr) {    // good point
                LOCAL_curr = LOCAL_next;
                LOCAL_fit_curr = LOCAL_fit_next;
                LOCAL_acnt++;                 // acceptance count increment
                if (LOCAL_fit_next<LOCAL_fit_best) {
                    LOCAL_best = LOCAL_next;
                    LOCAL_fit_best=LOCAL_fit_next;
                }
              }
              else if (LOCAL_fit_next>=LOCAL_fit_curr) { // bad point
                if (Pb(gen)) {
                    LOCAL_curr = LOCAL_next;
                    LOCAL_fit_curr = LOCAL_fit_next;
                    LOCAL_acnt++;
                }
              }
// MOVE COOLING OUTSIDE // I.E RUN 100 ITER AT SAME TEMPERATURE
              cooling(coolScheme, k , &temp);
// ------------------------ SEQUENTIAL SIMANN END ------------------------- //
              local_fit.next = LOCAL_fit_curr;
              local_fit.rank = id;
            //}
            MPI_Allreduce(&local_fit,&global_fit,1,MPI_DOUBLE_INT,MPI_MINLOC,MPI_COMM_WORLD);
            MPI_Allreduce(&LOCAL_acnt,&GLOBAL_acnt,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
            //MPI_Bcast(&global_fit,1,MPI_Double,0,MPI_COMM_WORLD);
// BROADCAST BEST POINT FROM BEST PROC TO EVERYONE ELSE AND REPEAT
            proc_with_best_ = global_fit.rank;
            MPI_Bcast(LOCAL_curr,dim,MPI_DOUBLE,proc_with_best_,MPI_COMM_WORLD);
          } while(GLOBAL_acnt <= 100 && global_fit.next > 0.1);

            // REANNEAL AND RESET PARAMETERS //
            reAnnCnt++;
            k = 1;
            temp = T0*0.95;
            LOCAL_acnt = 0;
            LOCAL_curr = LOCAL_best;
            LOCAL_fit_curr = LOCAL_fit_best;

      } while (iter < ITER_MAX && global_fit.next > 0.1 );
// ----------------END PARALLEL----------------- //
      MPI_Finalize();
// END TIME
      time_end=GetTimeMs64();
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

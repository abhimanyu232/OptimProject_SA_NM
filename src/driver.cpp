#include"optimisation.h"
#include <stdlib.h>     /* strtod */

double ITER_MAX = 1e9;           // default
double REPORT_INTERVAL = 100000; // default

int main(int argc, char *argv[]){

  if (argc == 1){
    std::cout << "default ITER_MAX:"<< ITER_MAX << '\n';;
  }
  else if (argc >= 2){
    ITER_MAX = strtod(argv[1],NULL);
    REPORT_INTERVAL = strtod(argv[2],NULL);
    std::cout << "ITER_MAX: " << ITER_MAX << '\n';
  }
  //
  //add method to choose dimension, optimisation method,
  //and fitness function from command line
  //

  // create directory for results
  char path1[] = "results";
  mkdir_p(path1);

  // enter problem dimension //
  int dim;
  cout << "enter search space dimension ( 2<= Dim <="<<DIM_MAX<<")"<<endl;
  while (!(cin >> dim) || dim < 2 || dim > DIM_MAX)
      cout<<" value out of range (2<=Dimension<="<< DIM_MAX << ")"<< '\n';

  // choice of optimisation algorithm //
  int opt_choice;
  cout << "Choose optimisation method \n"
  "1: Nelder Mead  \n2: Simulated Annealing" << endl;
  while (!(cin >> opt_choice) || (opt_choice !=1 && opt_choice != 2)){
      cout << " Invalid Choice \n"
      "1: Nelder Mead \n2: Simulated Annealing" << endl;
  }

  srand((unsigned int) time(0));

  fitVXd foo = NULL;        // function pointer for fitness function
  int testfcn = testFCN_choice(foo);      // choice of test function
  int bounds = domain_limit(testfcn);     // set domain bounds

  if (opt_choice == 1){ // Nelder Mead with Matrix input
      std::cout << "you chose nelderMead" << '\n';
      nelderMead(testfcn,bounds,dim,foo);
  }
  else if (opt_choice == 2){ //  simAnn with Vector input
      std::cout << "you chose simulated Annealing" << '\n';
      simAnnealing(testfcn,bounds,dim,foo);
  }
  else
  {std::cerr << "ERROR: invalid optimisation algo choice" << '\n'; return 0;}

return 0;
}

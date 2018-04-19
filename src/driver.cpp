#include"optimisation.h"


int main(){

  // enter problem dimension //
  int dim;
  cout << "enter search space dimension ( 2 <= Dim <= DIM_MAX ) \n" << endl;
  while (!(cin >> dim) || dim < 2 || dim > DIM_MAX)
    cout << " value out of range ( 2 <= Dimension <= " << DIM_MAX << " )" << '\n';

  // choice of optimisation algorithm //
  int opt_choice;
  cout << "Choose optimisation method \n"
  "1: Nelder Mead  \n2: Simulated Annealing" << endl;
  while (!(cin >> opt_choice) || opt_choice !=1 || opt_choice != 2){
    cout << " Invalid Choice \n"
    "1: Nelder Mead \n2: Simulated Annealing" << endl;
  }


    // set function pointer optimize to point to chosen optimisation algo.
    // declare appropriate function pointer foo for fitness evaluation.
  if (opt_choice == 1){
    static double (*foo)(int , const MatrixXd& ); // for NM with Matrix input
    // function pointer to point to nelder_mead
    static int (*optimize)(int, double (*)(int, const MatrixXd&)) = nelderMead;}
  else if (opt_choice == 2){
    static double (*foo)(int , const VectorXd& ); // for simAnn with Vector input
    // function pointer to point to simulated annealing
    static int (*optimize)(int, double (*)(int, const VectorXd&)) = simAnnealing;}
  else {std::cout << "ERROR: invalid optimisation algo choice" << '\n'; return 0;}


  // CHOICE OF TEST FUNCTION //
  cout << "Please choose the test function ; Enter 1 , 2 , 3 OR 4\n "
  "\tCase 1: Rosenbrock 2D \n\tCase 2:Rosenbrock Multidimensional \n\t"
  "Case 3: Sphere Multidimensional \n\tCase 4: Eggholder 2D" << endl;
  int choice;
  while (!(cin>>choice) || choice > 4 || choice < 1){
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    cout<<"please enter integers 1 2 3 or 4 only"<<endl;
    cin >> choice;
  }
  switch (choice) {
    case 1:
      foo = rosenbrock_2d;
      break;

    case 2:
      foo = rosenbrock_Nd;
      break;

    case 3:
      foo = sphere_Nd;
      break;

    case 4:
      foo = egghol;
      break;
  }

// call optimisation algo using
// optimize(dim, foo); calls the optimisation method selected above and passes it the
// selected fitness function.


  VectorXd X(dim);    // vector or matrix?
  X = VectorXd::Random(dim);
  cout <<"X: \n" << X << endl;

  double fit = foo(dim,X);
  cout << "fitness = " << fit << endl;
  optimize(dim,foo)
return 0;
}

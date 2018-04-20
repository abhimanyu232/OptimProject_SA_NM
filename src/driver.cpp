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
  while (!(cin >> opt_choice) || (opt_choice !=1 && opt_choice != 2)){
    cout << " Invalid Choice \n"
    "1: Nelder Mead \n2: Simulated Annealing" << endl;
  }

srand((unsigned int) time(0));
  if (opt_choice == 1){ // Nelder Mead with Matrix input
    static double (*foo)(int , const MatrixXd& );
    testFCN_choice(foo);
    std::cout << "this is nelderMead" << '\n';
    //nelderMead(dim,foo);
  }
  else if (opt_choice == 2){ //  simAnn with Vector input
    static double (*foo)(int , const VectorXd& );
    testFCN_choice(foo);
    std::cout << "this is simAnn" << '\n';
    //simAnnealing(dim,foo);
  }
  else {std::cout << "ERROR: invalid optimisation algo choice" << '\n'; return 0;}


  VectorXd X(dim);    // vector or matrix?
  X = VectorXd::Random(dim);
  cout <<"X: \n" << X << endl;

  //double fit = foo(dim,X);
  //cout << "fitness = " << fit << endl;

return 0;
}














void testFCN_choice(double (*fitness)(int, const MatrixXd&)){
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
    fitness = rosenbrock_2d;
    break;

  case 2:
    fitness = rosenbrock_Nd;
    break;

  case 3:
    fitness = sphere_Nd;
    break;

  case 4:
    fitness = egghol;
    break;
}
}

void testFCN_choice(double (*fitness)(int, const VectorXd&)){
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
    fitness = rosenbrock_2d;
    break;

  case 2:
    fitness = rosenbrock_Nd;
    break;

  case 3:
    fitness = sphere_Nd;
    break;

  case 4:
    fitness = egghol;
    break;
}
}

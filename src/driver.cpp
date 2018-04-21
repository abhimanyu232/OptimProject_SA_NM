#include"optimisation.h"


int main(){

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

  int testfcn;
  srand((unsigned int) time(0));

  if (opt_choice == 1){ // Nelder Mead with Matrix input
      double (*foo)(int , const MatrixXd& ) = NULL;
      testfcn = testFCN_choice(foo);
      std::cout << "this is nelderMead" << '\n';
      nelderMead(testfcn,dim,foo);
  }
  else if (opt_choice == 2){ //  simAnn with Vector input
      double (*foo)(int , const VectorXd& ) = NULL;
      testfcn=testFCN_choice(foo);
      std::cout << "this is simAnn" << '\n';
      simAnnealing(testfcn,dim,foo);
  }
  else
  {std::cerr << "ERROR: invalid optimisation algo choice" << '\n'; return 0;}


/*
  VectorXd X(dim);    // vector or matrix?
  X = VectorXd::Random(dim);
  cout <<"X: \n" << X << endl;
  //double fit = foo(dim,X);
  //cout << "fitness = " << fit << endl;
*/
return 0;
}




int testFCN_choice(fitMXd& fitness){
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
    return 1;
    break;

  case 2:
    fitness = rosenbrock_Nd;
    return 2;
    break;

  case 3:
    fitness = sphere_Nd;
    return 3;
    break;

  case 4:
    fitness = egghol;
    return 4;
    break;
}
return 0;
}

int testFCN_choice(fitVXd& fitness){
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
    return 1;
    break;

  case 2:
    fitness = rosenbrock_Nd;
    std::cout << "rosenbrock_Nd chosen" << '\n';
    return 2;
    break;

  case 3:
    fitness = sphere_Nd;
    return 3;
    break;

  case 4:
    fitness = egghol;
    return 4;
    break;
}
return 0;
}

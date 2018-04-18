#include"optimisation.h"


int main(){

int dim = 2;
cout << "enter search space dimension ( 2 <= Dim <= DIM_MAX ) \n" << endl;
while (!(cin >> dim) || dim < 2 || dim > DIM_MAX){
  cout << " value out of range ( 2 <= Dimension <= " << DIM_MAX << " )" << '\n';
}

double (*foo)(int  , const VectorXd& );

cout << "Please choose the test function ; Enter 1 , 2 , 3 OR 4\n "
"\tCase 1: Rosenbrock 2D \n\tCase 2:Rosenbrock Multidimensional \n\t"
"Case 3: Sphere Multidimensional \n\t Case 4: Eggholder 2D" << endl;
int choice;
while (!(cin>>choice) || choice > 4 || choice < 1){
  cin.clear();
  cin.ignore(1000,'\n');
  //cin.ignore(std::numeric_limits<std::streamsize>::max(),"\n");
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

// rough idea : implement with templates


VectorXd X(dim);    // vector or matrix? 
X = VectorXd::Random(dim);
cout <<"X: \n" << X << endl;

double fit = foo(dim,X);
cout << "fitness = " << fit << endl;

  return 0;
}

#include"optimisation.h"


int main(){

/*
// rough idea : implement with templates
template <typename Derived>
void (*foo)(const MatrixBase<Derived>&)
cout << "Please choose the test function ; Enter 1 , 2 , 3 OR 4\n "
"\tCase 1: Rosenbrock 2D \n\tCase 2:Rosenbrock Multidimensional \n\t"
"Case 3: Sphere Multidimensional \n\t Case 4: Eggholder 2D" << endl;
int choice;
while (!(cin>>choice) || choice > 4 || choice < 1){
  cin.clear()
  cin.ignore(std::numeric_limits<std::streamsize>::max(),"\n");
  cout<<"please enter integers only"<<endl;
  cin >> choice;
}
switch (choice) {
  case 1: foo = rosenbrock_2d;  break;
  case 2: foo = rosenbrock_Nd; break;
  case 3: foo = sphere_Nd; break;
  case 4: foo = egghol; break;
}

// rough idea : implement with templates
*/


Vector2d v1;
v1 << 1,1;
//v1 << Vector2d::Random();
cout <<"x :" << v1(0) << "y :" << v1(1)<< endl;
double fit = rosenbrock_2d(1,100,v1);
cout << "fitness = " << fit << endl;

  return 0;
}

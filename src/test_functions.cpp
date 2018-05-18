#include"optimisation.h"

//~~~~~~~~~~~Test Function Definitions~~~~~~~~~~~~~~~~~~~~//
// 2D Rosenbrock Function
// global minima f(x,y) = 0 at (x,y) = (a,a*a)
double  rosenbrock_2d(const int& dim, const VectorXd& X){
  assert(dim==2);
  double r2d =  pow(1 - X(0),2) + 100*pow(X(1) - X(0),2);
return r2d;
}

// Multidimensional Rosenbrock Function
// global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
double rosenbrock_Nd(const int& dim,const VectorXd& X){
  double rNd =0.;
  for (int i=0; i<dim-1;i++)
      rNd = rNd + ( 100*(pow(X(i+1) - pow(X(i),2),2)) + pow(1-X(i),2) );
return rNd;
}

// Multi Dimensional Sphere Function
// global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
double sphere_Nd(const int& dim,const VectorXd& X ){
  double sph = 0;
  for (int i =0; i<dim ; i++)
      sph = sph + pow(X(i),2);
return sph;
}

//2D Eggholder Function
//  global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
double egghol(const int& dim, const VectorXd& X){
  assert(dim == 2);
  assert((X(0)>=-512) && (X(1)<=512));
  double eggh = -1*(X(1)+47)*sin(sqrt(fabs(X(0)+(X(1)+47))))
                      - X(0)*sin(sqrt(fabs(X(0)-(X(1)+47))));
return eggh;
}

double schaf(const int& dim, const VectorXd& X){
  assert(dim==2);
  double sch = 0.5 + (pow(cos(pow(sin(fabs(X(0)*X(0)-X(1)*X(1))),2)),2) -0.5)/
    pow( (1+0.001*((X(0)*X(0)) + (X(1)*X(1)))), 2 );
return sch;
}

//~~~~~~~~~~~Test Function Choice~~~~~~~~~~~~~~~~~~~~//

int testFCN_choice(fitVXd& fitness){
cout << "Please choose the test function ; Enter 1 , 2 , 3 OR 4\n "
"\tCase 1: Rosenbrock 2D \n\tCase 2:Rosenbrock Multidimensional \n\t"
"Case 3: Sphere Multidimensional \n\tCase 4: Eggholder 2D\n\t"
"Case 5: Schaffer 2D"<< endl;
int choice;
while (!(cin>>choice) || choice > 5 || choice < 1){
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

  case 5:
    fitness = schaf;
    return 5;
    break;
}
return 0;
}

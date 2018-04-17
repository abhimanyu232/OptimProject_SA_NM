// 2D Rosenbrock Function
// global minima f(x,y) = 0 at (x,y) = (a,a*a)
//
template <typename Derived> // use Vector2d
double  rosenbrock_2d(double a, double b, const MatrixBase<Derived>& X){
  double r2d =  pow(a - X(0),2) + b*pow(X(1) - X(0),2);
return r2d;
}

// Multidimensional Rosenbrock Function
// global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
//
template <typename Derived> // use VectorXd
double rosenbrock_Nd(int dim,double a,double b,const MatrixBase<Derived>& X){
  double rNd =0.;
  for (int i=0; i<dim-1;i++)
      rNd = rNd + ( 100*(pow(X(i+1) - pow(X(i),2),2)) + pow(1-X(i),2) );
return rNd;
}

// Multi Dimensional Sphere Function
// global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
//
template <typename Derived>// use VectorNd
double sphere_Nd(int dim,const MatrixBase<Derived>& X){
  double sph = 0;
  for (int i =0; i<=dim ; i++)
      sph = sph + pow(X(i),2);
return sph;
}

//2D Eggholder Function
//  global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
//
template <typename Derived>// use Vector2d
double egghol(const MatrixBase<Derived>& X){
  double eggh = -1*(X(1)+47)*sin(sqrt(fabs(X(0)+(X(1)+47))))
                            - X(0)*sin(sqrt(fabs(X(0)-(X(1)+47))));
return eggh;
}

// Schaffer Function : has a plateau
// Cross in Tray or Himmebau : multiple global minimas

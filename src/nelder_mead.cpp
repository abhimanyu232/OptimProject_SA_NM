#include"optimisation.h"
#include"nelder_mead.h"


int nelderMead(const int& testfcn, const int& dim, fitVXd fit){
  
// use fit as a black box fitness function.
// where it points to is decided by user // switch statement from driver.cpp
// it takes arguement of types int, MatrixXd. SEE Function Definitions below.

// add any function/variables declarations that are needed only for the nelder
//mead method in nelder_mead.h // add corresponding declarations here //
// everything that needs to be accessed by driver.cpp needs to be declared in
// optimisation.h and defined in driver.cpp

// imo the points could be stored in matrix form with dim+1 rows and dim cols.
// it could be implemented as MatrixXd points(dim+1,dim)
// each row stores 1 point coordinates in dim dimensional space
// but feel free to use another method if you like


return 0;
}

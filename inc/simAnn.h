#include "optimisation.h"


// set initial temperature //
#define T0  1000    // initial temperature
#define MAX_STEP 20 // maximum random step size : range(-MAX_STEP,MAX_STEP)
#define REPORT_INTERVAL 100
#define K_BOLTZ 


bool cooling(const int choice, int k, double *temp);
void cooling_choice(int * choice);
float PAccept(double temp, double fit_curr, double fit_next);

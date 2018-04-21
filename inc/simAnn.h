#include "optimisation.h"


// set initial temperature //
#define T0  100
#define MAX_STEP 5 // maximum random step size : range(-MAX_STEP,MAX_STEP)
#define REPORT_INTERVAL 10

bool cooling(int choice, double temp);
void cooling_choice(int * choice);

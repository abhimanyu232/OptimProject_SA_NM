#include"optimisation.h"
#include"nelder_mead.h"
#include"get_time.h"

extern double ITER_MAX;
extern double REPORT_INTERVAL;

int nelderMead(const int& testfcn, const int& dim, fitVXd fit){

	if (testfcn > 5) {
		std::cout << "wrong test function. ERROR" << '\n';
		return 0;
	}
	std::cout << "dim ="<<dim << '\n';

	int64 time_begin,time_end;
// BEGIN TIME //
	time_begin = GetTimeMs64();

	int shrink=0,iter=0;
	Eigen::MatrixXd simplex; // dim+1,dim //
	simplex = MatrixXd::Random(dim+1,dim)*100;
	std::cout << "starting simplex: "<< '\n' <<simplex << '\n';

	Eigen::VectorXd fitness(dim+1);
	Eigen::VectorXd simplex_point(dim);

// calculate fitness of each point
	for (int i=0; i<=dim; i++){
		simplex_point = (simplex.row(i)).transpose();
		fitness(i) = fit(dim,simplex_point);
	}
std::cout << "fitness init: \n" << fitness << '\n';
	do {
		iter++;

//calculate the value at the new point
		if (shrink == 0 && iter>1){
			simplex_point = (simplex.row(dim)).transpose();
		 	fitness (dim) = fit(dim,simplex_point);
			std::cout << "IF ONE" << '\n';
		}

//shrinked -> calculate new values for every point but the former best
		else if (iter>1){		//
			for (int i=1; i<=dim; i++){
				simplex_point = (simplex.row(i)).transpose();
				fitness(i) = fit(dim,simplex_point);
			}
			std::cout << "IF TWO" << '\n';
		}

//reset shrink check
		shrink=0;

// sort simplex points in ascending order of fitness //
// potentially better implementation //
// row vector to store simplex row
		Eigen::RowVectorXd temp_simplex;
		double temp_fitness;
		for(int i=0;i<dim;i++){
			 for(int j=0;j<dim-i;j++){
				 if(fitness(j)>fitness(j+1)){
					 temp_simplex=simplex.row(j);
					 simplex.row(j)=simplex.row(j+1);
					 simplex.row(j+1)=temp_simplex;
					 temp_fitness=fitness(j);
					 fitness(j)=fitness(j+1);
					 fitness(j+1)=temp_fitness;
					 std::cout << "IF THREE" << '\n';
				 }
			 }
		 }

		 Eigen::VectorXd m(dim), r(dim), c(dim), cc(dim), s(dim);
		 double fitness_cc,fitness_ext,fitness_refl,fitness_contr;
// barycentre coordinates //
		MatrixXd simplex_formean(dim,dim);
	 	for (int j=0; j<dim; j++){
		 simplex_formean.row(j)=simplex.row(j);
	 	}
		for (int j=0; j<dim; j++){
		 		m(j)=(simplex_formean.col(j)).mean();
		}

//create reflect point
		 r = 2*m - (simplex.row(dim)).transpose();
		 fitness_refl = fit(dim,r);
		 if ( fitness_refl<fitness(dim-1) && fitness_refl>=fitness(0) ){
		 		simplex.row(dim) = r.transpose(); // reflect and end iteration //
				std::cout << "REFLECTED" << '\n';
		 }
//create extend point
		 else if (fitness_refl<fitness(0)){
				 s = m + 2*( m - (simplex.row(dim)).transpose() );
				 fitness_ext = fit(dim,s);
				 if (fitness_ext<fitness_refl){
					 simplex.row(dim) = s.transpose(); //extend
					 std::cout << "EXTEND" << '\n';
				 }
				 else {
					 simplex.row(dim) = r.transpose(); //reflect
					 	std::cout << "REFLECTED" << '\n';
				 }
		 }

//create contract outside point
		 else if ( fitness_refl >= fitness(dim-1) ){
		 		 if ( fitness_refl < fitness(dim) ){ // betweeen the two worst points
			 	 	 c = m + (r-m)/2;  	   						 //contract outside
					 fitness_contr = fit(dim,c);
					 if (fitness_contr < fitness_refl){
						  simplex.row(dim) = c.transpose();
					 }
					 else { 														//shrink
					    for (int i=1; i<dim+1; i++){
							 	 simplex.row(i) = simplex.row(0) +
							 									 (simplex.row(i)- simplex.row(0))/2;
								 shrink=1;
						  }
					 }
			   }
				 std::cout << "IF 6" << '\n';
			}

//create contract inside point
			else {                            		// worse or equal to worst current
				 cc = m + ((simplex.row(dim)).transpose() - m)/2;
				 fitness_cc = fit(dim,cc);
				 if ( fitness_cc<fitness(dim) ){
					  simplex.row(dim) = cc.transpose();
						std::cout << "IF 7" << '\n';
				 }
//shrink
				 else	{
					  for (int i=1; i<dim+1; i++){
							 simplex.row(i) = simplex.row(0) +
							 								 (simplex.row(i)- simplex.row(0))/2;
							 shrink=1;
							 std::cout << "IF 8" << '\n';
						}
				 }
			}
	// print to screen and file
	if (fmod(iter,REPORT_INTERVAL)==0){
	std::cout << "iter: "<< iter << "\t\t" << "best_fitness:" << fitness(0) << '\n';
	std::cout << "simplex " << iter << ":\n" << simplex << '\n';
	}//
	}	while (iter <= ITER_MAX);

	time_end = GetTimeMs64();
	std::cout << "Time Elapsed: " << time_end - time_begin << "ms" << '\n';

return 0;
}

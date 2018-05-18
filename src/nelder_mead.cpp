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
	simplex = MatrixXd::Random(dim+1,dim)*1000;
	std::cout << "starting simplex: "<< '\n' <<simplex << '\n';

	Eigen::VectorXd fitness(dim+1);
	Eigen::VectorXd simplex_point(dim);

	// calculate fitness of each point
	for (int i=0; i<=dim; i++){
		simplex_point = (simplex.row(i)).transpose();
		fitness(i) = fit(dim,simplex_point);
	}

	do {
		iter++;
		// take the worst point //
		if (shrink == 0 && iter>1){
			simplex_point = (simplex.row(dim)).transpose();
		 	fitness (dim) = fit(dim,simplex_point);
		}
		// something like this to find the worst point
		// for (int i=0;i<=dim;i++){
		// 	worst_fit = fitness(i)
	  // 	if (fitness(i)<worst_fit){
		// 		worst_fit = fitness(i)
		//  	worst_point  = i ;
		//  	}
		// simplex_point = simplex.row(WORST_POINT)
		// }
		//}
		else if (iter>1){		//
			for (int i=1; i<=dim; i++){
				simplex_point = (simplex.row(i)).transpose();
				fitness(i) = fit(dim,simplex_point);
			}
		}
		shrink=0;
		double temp_fitness;
		Eigen::VectorXd temp_simplex;
		// sort simplex points in ascending order of fitness //
		// potentially better implementation //
		for(int i=0;i<=dim;i++){
			 for(int j=0;j<dim-1-i;j++){
				 if(fitness(j)>fitness(j+1)){
					 temp_simplex=simplex.row(j);
					 simplex.row(j)=simplex.row(j+1);
					 simplex.row(j+1)=temp_simplex;
					 temp_fitness=fitness(j);
					 fitness(j)=fitness(j+1);
					 fitness(j+1)=temp_fitness;
				 }
			 }
		 }

		 Eigen::VectorXd m(dim), r(dim), c(dim), cc(dim), s(dim);
		 double fitness_cc,fitness_ext,fitness_refl,fitness_contr;
		 // barycentre coordinates //
		 for (int j=0; j<dim; j++){
		 		m(j)=(simplex.col(j)).mean();
		 }
		 r = 2*m - (simplex.row(dim)).transpose(); // reflect
		 fitness_refl = fit(dim,r);
		 if ( fitness(dim-1)>fitness_refl && fitness_refl>=fitness(0) ){
		 		simplex.row(dim) = r.transpose();
		 }
		 else if (fitness_refl<fitness(0)){
				 s=m + 2*( m- (simplex.row(dim)).transpose() ); //extend
				 fitness_ext = fit(dim,s);
				 if (fitness_ext<fitness_refl){
					 simplex.row(dim) = s.transpose();
				 }
				 else {
					 simplex.row(dim) = r.transpose();
				 }
		 }
		 else if ( fitness_refl >= fitness(dim-1) ){
		 		 if ( fitness_refl < fitness(dim) ){ // betweeen the two worst points
					 c = m + (r-m)/2;  //contract outside
					 fitness_contr = fit(dim,c);
					 if (fitness_contr < fitness_refl){
						  simplex.row(dim) = c.transpose();
					 }
					 else {
					    for (int i=1; i<dim+1; i++){
							 	 simplex.row(i) = simplex.row(0) +
							 									 (simplex.row(i)- simplex.row(0))/2;
								 shrink=1;
						  }
					 }
			   }
			}
			else {                                 // worse than worst current
				 cc = m + ((simplex.row(dim)).transpose() - m)/2; // contract inside
				 fitness_cc = fit(dim,cc);
				 if ( fitness_cc<fitness(dim) ){
					  simplex.row(dim) = cc.transpose();
				 }
				 else	{
					  for (int i=1; i<dim+1; i++){
							 simplex.row(i) = simplex.row(0) +
							 								 (simplex.row(i)- simplex.row(0))/2;
							 shrink=1;
						}
				 }
			}
	// print to screen and file
	std::cout << "iter: "<< iter << '\t' << "best fitness:" << fitness(0) << '\n';
	//
	}	while (iter <= ITER_MAX);

	time_end = GetTimeMs64();
	std::cout << "Time Elapsed: " << time_end - time_begin << "ms" << '\n';

return 0;
}

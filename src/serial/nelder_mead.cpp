#include"optimisation.h"
#include"nelder_mead.h"
#include"get_time.h"

extern double ITER_MAX;
extern double REPORT_INTERVAL;

int nelderMead(const int& testfcn, const int& bounds,const int& dim, fitVXd fit){
	if (testfcn > 5) {
		std::cout << "wrong test function. ERROR" << '\n';
		return 0;
	}

  ofstream result_file("results/NMead.dat");
  if ( result_file.is_open() ){
    result_file << "Iteration \t Fitness Value " << endl ;
    result_file.close();
  }
	else {cerr << "ERROR OPENING DAT FILE\n";}

	int64 time_begin,time_end;
// BEGIN TIME //
	time_begin = GetTimeMs64();

	int shrink=0,iter=0;
	Eigen::MatrixXd simplex; // dim+1,dim //
	simplex = MatrixXd::Random(dim+1,dim)*bounds;
	std::cout << "starting simplex: "<< '\n' <<simplex << '\n';

	Eigen::VectorXd fitness(dim+1);
	Eigen::VectorXd simplex_point(dim);

// calculate fitness of each point
	for (int i=0; i<=dim; i++){
		simplex_point = (simplex.row(i)).transpose();
		fitness(i) = fit(dim,simplex_point);
	}

	std::cout << "fitness init: \n" << fitness << '\n';
	result_file.open("results/NMead.dat", ios::app);
	if (!result_file.is_open()){
		cerr << "unable to write data to file" << '\n';
		return 0;
	}

	do {
		iter++;

//calculate the value at the new point and replace the previous worst
		if (iter>1){
			if (shrink == 0){
					simplex_point = (simplex.row(dim)).transpose();
		 			fitness (dim) = fit(dim,simplex_point);
			}
//shrinked -> calculate new values for every point but the former best
			else if (shrink==1){
				for (int i=1; i<=dim; i++){
					simplex_point = (simplex.row(i)).transpose();
					fitness(i) = fit(dim,simplex_point);
				}
			}
		}
//reset shrink check
		shrink=0;

// sort simplex points in ascending order of fitness //
// potentially better implementation //
		 sort_simplex(dim, fitness, simplex);

		 Eigen::VectorXd m(dim), r(dim), c(dim), cc(dim), s(dim), worst(dim);
		 double fitness_cc,fitness_ext,fitness_refl,fitness_contr;
		 worst = (simplex.row(dim)).transpose();

// CALCULATE BARYCENTER //
		 find_barycentre(dim,m,simplex);

		 double alpha = 1;										// standard value
		 double gamma = 2;										// standard value
		 double beta = 0.5;										// standard value
		 double rho = 0.5;										// standard value
		 //assert(gamma >1 && beta < 1 && rho < 1);
// REFLECTED POINT //
		 r = (1+alpha)*m - alpha*worst;
		 fitness_refl = fit(dim,r);
// ACCEPT REFLECTION //
		 if ( fitness_refl<fitness(dim-1) && fitness_refl>=fitness(0) ){
		 		simplex.row(dim) = r.transpose();
		 }
// CONSIDER EXTENSION //
		 else if (fitness_refl<fitness(0)){
		 	   s = m + gamma*(r - m);
				 //s = m + 2*( m - worst );
				 fitness_ext = fit(dim,s);
// ACCEPT EXTENSION //
				 if (fitness_ext<fitness_refl){
					 simplex.row(dim) = s.transpose(); //extend
//					 std::cout << "EXTEND" << '\n';
				 }
// REJECT EXTENSION AND REFLECT //
				 else {
					 simplex.row(dim) = r.transpose();
//					 	std::cout << "REFLECTED" << '\n';
				 }
		 }
// CONTRACT OR SHRINK //
		 else if ( fitness_refl >= fitness(dim-1) ){
// REFLECTION BETWEEN WORST TWO POINTS : CONTRACT OUTSIDE
		 		 if ( fitness_refl < fitness(dim) ){
				 	 	 	c = m + beta*(r-m);
						 	fitness_contr = fit(dim,c);
						 	if (fitness_contr < fitness_refl){
							  	simplex.row(dim) = c.transpose();
//									std::cout << "CONTRACT OUTSIDE" << '\n';
						  }
// SHRINK IF CONTRACTION FAILS //
				 		  else {
										shrink_simplex(dim,rho,shrink,simplex);
							}
				 }
// REFLECTION WORST POINT YET : CONTRACT INSIDE
			 	 else if ( fitness_refl >= fitness(dim) ){ // WORST CASE
			 				cc = m - beta*( m - worst);
							fitness_cc = fit(dim,cc);
							if ( fitness_cc<fitness(dim) ){ // ACCEPT CONTRACT INSIDE
									 simplex.row(dim) = cc.transpose();
//									 std::cout << "CONTRACT INSIDE" << '\n';
							}
// SHRINK IF CONTRACTION FAILS //
							else {
										shrink_simplex(dim,rho,shrink,simplex);
							}
				 }
		 }

// PRINT SCREEN
		 if (fmod(iter,REPORT_INTERVAL)==0){
			 cout<<"iter: "<<iter<<"\t\t"<<"best_fitness:"<<fitness(0)<<'\n';
			 cout<<"simplex "<<iter<<":\n"<<simplex<<'\n';
		 }
// WRITE TO FILE
		 if (fmod(iter,REPORT_INTERVAL)==0){
			 if (result_file){
			 		result_file << setw(9) <<iter<<"\t"<<
			 		setprecision(10)<<setw(13)<<fitness(0)<< endl ;
			 }
			 else cerr << "Error writing to file on iter:"<<iter<< '\n';
		 }

	}	while (iter <= ITER_MAX && fitness(0) > 1e-3);

	time_end = GetTimeMs64();
	cout << "Time Elapsed: " << time_end - time_begin << "ms" << '\n';

	cout <<'\n' << "BEST POINT:"<< '\n';
	cout << simplex << '\n';
	std::cout << "BEST FITNESS: "<< fitness(0) << '\n';
return 0;
}

// SORT SIMPLEX POINTS IN ASC ORDER OF FITNESS : simplex(dim) is worst point
	void sort_simplex(const int& dim, VectorXd& fitness, MatrixXd& simplex){
		double temp_fitness;
		RowVectorXd temp_simplex;
		for(int i=0;i<dim;i++){
			 for(int j=0;j<dim-i;j++){
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
 return;
 }
//

//
 void shrink_simplex(const int& dim,const double& rho,int& shrink, MatrixXd& simplex){
	 for (int i=1; i<dim+1; i++){
				simplex.row(i) = simplex.row(0) +
												rho*(simplex.row(i)- simplex.row(0));
				shrink=1;
	 }
 return;
 }
//

//
 void find_barycentre(const int& dim, VectorXd& mean, MatrixXd simplex){
	 MatrixXd simplex_formean(dim,dim);
	 for (int j=0; j<dim; j++)
				simplex_formean.row(j)=simplex.row(j);

	 for (int j=0; j<dim; j++)
				mean(j)=(simplex_formean.col(j)).mean();
 return;
 }
//

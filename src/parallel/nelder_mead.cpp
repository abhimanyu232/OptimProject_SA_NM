#include"optimisation.h"
#include"nelder_mead.h"
#include<mpi.h>

double ITER_MAX = 10000;
double REPORT_INTERVAL = 500;

int main (int argc, char* argv[]){

    MPI_Init(&argc,&argv);
    int size, id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc == 1){
    if (!id)
    std::cout << "default ITER_MAX:"<< ITER_MAX << '\n';;
  }
  else if (argc >= 3){
    ITER_MAX = strtod(argv[1],NULL);
    REPORT_INTERVAL = strtod(argv[2],NULL);
    if (!id)
    std::cout << "ITER_MAX: " << ITER_MAX << "\tREPORT INTERVAL :" << REPORT_INTERVAL<< '\n';
  }

// TAKE COMMAND LINE ARGUEMENT TO REDUCE COMMUNICATION
    int dim;
    if (id == 0){
      cout << "enter search space dimension ( 2<= Dim <="<<DIM_MAX<<")"<<endl;
      while (!(cin >> dim) || dim < 2 || dim > DIM_MAX)
      cout<<" value out of range (2<=Dimension<="<< DIM_MAX << ")"<< '\n';
    }
    MPI_Bcast(&dim,1,MPI_INT,0,MPI_COMM_WORLD);

    if (dim==0)
    MPI_Finalize();

    srand((unsigned int) time(0));

    fitVXd fit = NULL;        // function pointer for fitness function
		// change function to take testfcn as arguement to remove the user input
		// 1 : Rastrigin // 2 : Rosenbrock
    int testfcn = 1;
    P_testFCN_choice(testfcn,fit);      // choice of test function
    int bounds = domain_limit(testfcn);     // set domain bounds

    struct {
      double next;
      int rank;
    }local_fit,global_fit;


	if (id==0){
	  ofstream result_file("results/NMead.dat");
	  if ( result_file.is_open() ){
	    result_file << "Iteration \t Fitness Value " << endl ;
	  }
		else {cerr << "ERROR OPENING DAT FILE\n";}
	}

// BEGIN TIME //
  MPI_Barrier(MPI_COMM_WORLD);
  auto time_begin = Clock::now();

	int shrink=0,iter=0;
	Eigen::MatrixXd simplex; // dim+1,dim //
	simplex = MatrixXd::Random(dim+1,dim)*bounds;//*((id+1)/size);
	//std::cout << "starting simplex: "<< '\n' <<simplex << '\n';

	Eigen::VectorXd fitness(dim+1), simplex_point(dim);

	double best_fit_last_iter, glbl_best;
	int counter=0;
// calculate fitness of each point
	for (int i=0; i<=dim; i++){
		simplex_point = (simplex.row(i)).transpose();
		fitness(i) = fit(dim,simplex_point);
	}
	sort_simplex(dim, fitness, simplex);
	glbl_best = fitness(0);

	//std::cout << "fitness init: \n" << fitness << '\n';

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

		best_fit_last_iter = fitness(0);
// sort simplex points in ascending order of fitness //
// potentially better implementation //
		 sort_simplex(dim, fitness, simplex);
		 if ( best_fit_last_iter == fitness(0) ){ counter++; } else { counter=0; }

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
		 if (fitness(0)<glbl_best)
		 glbl_best=fitness(0);

// PRINT SCREEN
/*		 if (fmod(iter,REPORT_INTERVAL)==0){
			 cout<<"iter: "<<iter<<"\t\t"<<"best_fitness:"<<fitness(0)<<'\n';
			 cout<<"simplex "<<iter<<":\n"<<'\n';
			 std::cout << "GLOBAL BEST:" << glbl_best << '\n';
		 }

// WRITE TO FILE
	 if (fmod(iter,REPORT_INTERVAL)==0){
			 if (result_file){
			 		result_file << setw(9) <<iter<<"\t"<<
			 		setprecision(10)<<setw(13)<<fitness(0)<< endl ;
			 }
			 else cerr << "Error writing to file on iter:"<<iter<< '\n';
		 }
*/

		 local_fit.next = glbl_best;
		 local_fit.rank = id;
	 //}
	 MPI_Allreduce(&local_fit,&global_fit,1,MPI_DOUBLE_INT,MPI_MINLOC,MPI_COMM_WORLD);
	 if (!id){
		 if (fmod(iter,1000)==0)
 		 std::cout << "global best across proc:" << global_fit.next << '\n';
		 //std::cout << "iter : "<< iter << '\n';
	 }
	 if (fmod(iter,1000)==0)
	 std::cout << "fitness local for proc" << id << ":" << fitness(0) << '\n';

// RESETT SIMPLEX IF STUCK IN LOCAL MINIMA //
/*		 if (counter == 10){
			 	simplex = MatrixXd::Random(dim+1,dim)*bounds;
			 	for (int i=0; i<=dim; i++){
			 		simplex_point = (simplex.row(i)).transpose();
			 		fitness(i) = fit(dim,simplex_point);
					counter = 0;
			 	}
		 }
*/
	} while ( global_fit.next  > 0.1);

	MPI_Barrier(MPI_COMM_WORLD);
	auto time_end = Clock::now();
	std::chrono::duration<double> time_elapsed = time_end-time_begin;
	cout << "Time Elapsed: " << time_elapsed.count() << "ms" << '\n';

	MPI_Barrier(MPI_COMM_WORLD);
	if (!id){
	cout <<'\n' << "BEST POINT:"<< '\n';
	//cout << simplex << '\n';
	std::cout << "BEST FITNESS: "<< global_fit.next << '\n';
	}

	MPI_Finalize();
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

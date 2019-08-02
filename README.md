## Compilation and Execution 

* Place completed .cpp files in *src* folder. Any file not in the src folder *will not* be compiled.

* Place necessary header (.h) files in the *inc* folder

type **make** from terminal to compile sequential. 

type **make psa** from terminal to compile parallel simulated annealing. 

type **make pnm** from terminal to compile parallel nelder mead. 

type **./optimize.out** in terminal to run the sequential executable. 

type **mpiexec -np [no:s of processors] ./psa.out [Max_Iter] [Report Interval] [Test Function No.]** in terminal to run the parallel simulated annealing executable. 

type **mpiexec -np [no:s of processors] ./pnm.out [Max_Iter] [Report Interval] [Test Function No.]** in terminal to run the parallel nelder mead executable. 

type **make clean** to clear all compiled executable files. 

type **make clean_all** to clear all result & executable files.    






 
saR <- read.table("C:/Users/Abhimanyu.MARVIN-UNDEAD/Desktop/saR.dat")
saRas <- read.table("C:/Users/Abhimanyu.MARVIN-UNDEAD/Desktop/saRas.txt")

nmras5 <- read.table("C:/Users/Abhimanyu.MARVIN-UNDEAD/Desktop/nmras5.txt")
nmras6 <- read.table("C:/Users/Abhimanyu.MARVIN-UNDEAD/Desktop/nmras6.txt")

nmros20 <- read.table("C:/Users/Abhimanyu.MARVIN-UNDEAD/Desktop/nmros20.txt")
nmros22 <- read.table("C:/Users/Abhimanyu.MARVIN-UNDEAD/Desktop/nmros22.txt")

paras <- read.table("C:/Users/Abhimanyu.MARVIN-UNDEAD/Desktop/paras.txt")

nmdet <- read.table("C:/Users/Abhimanyu.MARVIN-UNDEAD/Desktop/nm_deter.txt")
?boxplot

boxplot(saR,log="y",names=c("Matlab","C++"), ylab="Time (s)",main="Simulated Annealing Rosenbrock (d=30)")
boxplot(saRas,names=c("Matlab","C++"), ylab="Time (s)",main="Simulated Annealing Rastrigin (d=10)")
boxplot(nmros20,log="y",names=c("Matlab","C++"), ylab="Time (s)",main="Nelder Mead Rosenbrock (d=20)")
boxplot(nmros22,log="y",names=c("Matlab","C++"), ylab="Time (s)",main="Nelder Mead Rosenbrock (d=22)")
boxplot(nmras5,log="y",names=c("Matlab","C++"), ylab="Time (s)",main="Nelder Mead Rastrigin (d=5)")
boxplot(nmras6,log="y",names=c("Matlab","C++"), ylab="Time (s)",main="Nelder Mead Rastrigin (d=6)")

boxplot(paras,names=c("Matlab","C++", "MPI + C++"), ylab="Time (s)",main="Parallel Implentation : Simulated Annealing (d=10)")
boxplot(nmdet,names=c("Matlab D10","C++ D10", "Matlab D13" , "C++ D13"), ylab="Time (s)",main="Nelder Mead without Random Restart")

paras

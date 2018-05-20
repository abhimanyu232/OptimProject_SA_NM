%data = dlmread('results/Sim_Ann.dat', '\t', 1,0);
data = dlmread('results/NMead.dat', '\t', 1,0);
%sort_fit = sort(data(:,2));
N=length(data);
close()
figure();
sz = 15;
c = linspace(1,10,length(data));
scatter(data(:,1),data(:,2),sz,'c','filled','o');
xlim([100 data(N,1)])
xlabel("Iterations"); ylabel("Fitness Value")
%ylim([0 100 ])
title("Fitness Value Evaluation vs. Iterations");
grid()

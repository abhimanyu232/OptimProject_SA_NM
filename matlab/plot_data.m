% data = dlmread('results/Sim_Ann.dat', '\t', 1,0);
data = dlmread('M:\0_Code\Projects\OptimisationProject\results\NMead.dat', '\t', 1,0);
%sort_fit = sort(data(:,2));
N=length(data);
close()
figure();
sz = 15;
c = linspace(1,10,length(data));
scatter(data(:,1),log10(data(:,2)),sz,'c','filled','o');
% xlim([100 data(N,1)])
xlabel("Iterations"); ylabel("log10 Fitness Value")
% ylim([0 100 ])
title("Fitness Value Evaluation vs. Iterations");
grid()

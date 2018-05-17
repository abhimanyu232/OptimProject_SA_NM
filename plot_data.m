data = dlmread('results/Sim_Ann.dat', '\t', 1,0);
% data = dlmread('results/NMead.dat', '\t', 1,0);
sort_fit = sort(data(:,2));
N=length(data);
close()
figure();
sz = 10;
c = linspace(1,10,length(data));
scatter(data(:,1),data(:,2),sz,'red','filled');
ylim([0 100 ])
title("Fitness Value Evaluation vs. Iterations");
grid()

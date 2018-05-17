data = dlmread('Sim_Ann.dat', '\t');
N=length(data);
close()
figure();
sz = 10;
% c = linspace(1,10,length(SimAnn));
scatter(data(:,1),data(:,2),sz,'filled');
ylim([0 1000 ])
title("Fitness Value Evaluation vs. Iterations");
grid()

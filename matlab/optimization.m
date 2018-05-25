clear all

n=2;

%Rosenbrock 2D
%n=2;
% fun = @(x)100*(x(2) - x(1)^2)^2 + (1 - x(1))^2;


%Eggholder
%n=2;
%fun = @(x)-1*(x(2)+47)*sin(sqrt(abs(x(1)+(x(2)+47))))...
%                       - x(1)*sin(sqrt(abs(x(1)-(x(2)+47))));

%Schaf
%n=2;
% fun = @(x)0.5 + (cos(sin(abs(x(1)*x(1)-x(2)*x(2)))^2)^2 -0.5)/...
%      (1+0.001*((x(1)*x(1)) + (x(2)*x(2))))^2;
for n = 10:4:26
    for i=1:10
x0 = rand(1,n); 
 
%Rosenbrock ND
x=x0;
f=@(x,n)RosenbrockNd(x,n);
fun=@(x)f(x,n);

% Simulated Annealing
options = optimoptions(@simulannealbnd,'FunctionTolerance',10^-10);
% options = saoptimset('PlotFcns',{@saplotbestx,...
%           @saplotbestf,@saplotx,@saplotf});
lb(1:n)=-1000 ;
ub(1:n)=1000;
tic
[SAresult,fval] = simulannealbnd(fun,x0,lb,ub,options);
tSA=toc
time(n,i)= tSA*1000  
    end
end

%Nelder Mead
% 
% tic
% NMresult = fminsearch(fun,x0);
% tNM=toc
% time(n,i)= tNM*1000  


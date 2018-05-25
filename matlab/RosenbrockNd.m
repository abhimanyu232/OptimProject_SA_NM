function f = RosenbrockNd(x,n)
f=0;
for i = 1:n-1
    f = f + 100*(x(i+1) - x(i)^2)^2 + (1 - x(i))^2;
end
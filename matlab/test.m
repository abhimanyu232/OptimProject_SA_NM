clear all
T0 = 1000

for (i=1:400)
    temp1(i) = T0*0.99^(i-1);
end

for (i=1:400)
    temp2(i) = T0*0.87^(i-1);
end

% for (i=1:200)
%     temp2(i) = T0/(0.2*i);
% end

% for (i=1:200)
%     temp3(i) = T0/log10(i);
% end

close all
figure(1);
title("Cooling Scheme")
hold()
plot(temp1,'r','linewidth',2.0)
plot(temp2,'b','linewidth',2.0)
% plot(temp3,"g.")
grid()
xlabel("Iterations")
ylabel("Temperature")
legend("T0*pow(0.99,k)","T0*pow(0.87,k)")
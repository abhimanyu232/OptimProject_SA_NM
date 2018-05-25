
x = simulannealbnd(@Rosenb,x)



% 
% t1 = 1000
% for i=1:100
%   % t(i)= t1*(0.97^i);
%    t(i)= t1*(1/log(i+2))
% end
% t
% plot(t)



function f = Rosenb(x)
f = 100*(x(:,2) - x(:,1).^2).^2 + (1 - x(:,1)).^2;
end

function f = RosenND(x)

end 

function f = SphereND(x)

end 

function f = Eggholder()

end 

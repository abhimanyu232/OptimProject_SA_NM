function rstgnND = RastriginNd(x,n)
  rstgnND = 10*n ;
  for i = 1:n 
      rstgnND = rstgnND + ( x(i)^2 - 10*cos(2*pi*x(i)) ) ;
  end
  
  end
  
      
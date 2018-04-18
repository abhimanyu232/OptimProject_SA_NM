
// simulated annealing //

VectorXd = Matrix<double, dim, 1>
RowVectorXd = Matrix<double,1,dim>

RowVectorXd curr, new, best_pt


1.  Temperature = 100 ; curr = random() ; best_pt = curr;
do {
2. new = curr  + delta //
3. if fit(new) <fit(curr)
        curr  = new
        if fit(new) < fit(best)
        best = curr
else if fit(new) > fit(curr) && p(k,Temp) > rand()
           curr =  new
          }
4. Temp = Temp*whatever [cooling scheme]
  }

// REANNEALING??

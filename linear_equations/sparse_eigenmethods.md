# Introduction:
At the request of a friend doing physics research,
I have been poised the general question of can I speed up eigenpair finding in the sparse case.
I have narrowed this search to the problem in what I believe to be referred to the asymptotic sparse diagonalizable case.

### Why Diagonalizable
Diagonalizable matricies are dense in the Complex field.
I doubt the algorithms are precise enough without this assumption.

### Definition Asymptotically Sparse Matrix:
A Class of matricies is asymptotically sparse 
iff the number of nonzero entries per row can be bounded above by a constant s.
Matricies of asymptotically sparse classes can be multiplied with a vector in time O(ns)
as opposed to time O(n^2)
### Target Remark: 
I believe the work complexity of O(Csn^2) can be achieved with C 
being a constant of the numerical stability of the problem posed. 
For some problems, this will be a significant speedup over dense methods.
A depth Complexity of O(Cpoly(log(n),log(s)) ) should be acheived with this same method.
### Stability Remark:
For the purpose of this first draft, 
I have not successfully bound the necessary number of iterations,
and thus the constant C is unknown.

# Eigenvalue Problem:
### Reduction 1:
The first reduction is well established relationship between the characteristic polynomial
```
eigenvalues(A):
  coefficients_polynomial = characteristic_polynomial(A)
  return zeros_finding(coefficicents_polynomial)
```
### Reduction 2:
The second reduction is well established relationship of a polynomial's evaluation points and its coefficients.
We are choosing the sample points so that later parts of the algorithm can use an easy inversion choice.
```
characteristic_polynomial(A):
  N = 2_norm_estimate(A)
  B = 2 // a number bigger than 1 which effects truncation error vs convergence error
  x = list_roots_of_identity(n(A) * B * N
  sampled_polynomial = sampled_determinant(A,x )
  return inverse_vandermonde(x,sampled_polynomial) // if done correctly this is a scaled fourier transform
```

### Reduction 3:
The problem is to solve det(xI-A) for varying x.
We expand out the problem along the diagonal using a cheap formula for (I-e)^-1
The formula for expanding along the diagonal comes from the block matrix formula.
```
sampled_determinant(A,x):
  dets = x hadmard_power n // might be better as just (NB)^n
  for i in 1 to n : // easily parallelized
    dets = dets hadmard_product ith_term_determinant(A,x,i) 
  return dets
ith_term_determinant(A,x,i):
  product = I * (column i up to row i-1 (A))
  initialize z 
  for j in 1 to p:
    z_j = (row i up to column i-1(A)) * product
    product = A matrix_multiply product
  y = Vandermonde_multiply(entrywise_inverse(x), z)
  return 1_n - (A_ii) * entrywise_inverse(x) - y
```
### Analysis
Work is O(nlogn + nlogn + n^2logn +  + psn^2) for the four layers of heavy logic.
Note that I as of writing don't know how to bound the p term.
Clearly as p -> infinity, the inverse formula holds.
How large p should be for convergence of the eigenvalues themselves is unclear.








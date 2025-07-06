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
  product = (column i up to row i-1 (A))
  initialize z to be empty
  for j in 1 to p:
    z_j = (row i up to column i-1(A)) * product
    product = A * product
  y = Vandermonde_multiply(entrywise_inverse(x), z)
  return 1_n - (A_ii) * entrywise_inverse(x) - y
```
### Analysis
Work is O(nlogn + nlogn + n^2logn +  + psn^2) for the four layers of heavy logic.
Note that I as of writing don't know how to bound the p term.
Clearly as p -> infinity, the inverse formula holds.
How large p should be for convergence of the eigenvalues themselves is unclear at time of writing

# Eigenvector Problem:
### Reduction 1:
This first reduction follows from the definition of an eigenvector.
```
eigenvector(lambda,m,A):
  return nullspace(A- lambda * I_n,m)
```
### Reduction 2:
The second reduction is from nullspace to random sampling:
#### WARNING: If we need an orthogonal vector set within the same eigenspace that will be MORE work
```
nullspace(B, k):
  for i in 1 to k:
    r_k = random_vector(n) //sample unit normal vector from uniform spherical distribution.
    n_k = least_squares_one_b(A,r_k,1) //solve Ax = 0 ; r_kTx = 1
  return n_*

// This method could be lifted to be a general sparse system solver... of s_b * n * s_A * C work...
least_squares_one_b(A,c,b):
  //ASSUMING A is both s-row-asymptotic-sparse AND s-column-asymptotic-sparse:
  A_* = break_into_orthogonal_groups(A)
  // Using graph theory on the matrix AAT you want an independent set.
  // So coloring that graph with s^2 colors gets you the division.
  // A lazy bad division is all rows separately.
  for all i:
    let a_i be the row 2-norms of A_i
  v = c //start here
  i = 1
  //using epsilon for precision.
  while 2-norm(A*v) > epsilon :
    v = v - (A_iT * ( Diag(entrywise_inverse(a_i hadmard a_i) ) (A_i * v)))
    normalization_constant = cT * v
    if abs( normalization_constant)  < epsilon * sqrt(vT*v)  :
      throw NO VECTOR ERROR
    v = v * b / normalization_constant
    i++
    if i > number_of_orthogonal_groups :
      i = 1
  return v 
```
### Concerns:
The argument which is most dubious is:
Is the random sampling sufficient to get the whole nullspace?
Other Concern:
Clearly, if a vector is returned its an approximate nullspace vector.
However, It might not be obvious to everyone why the algorithm converges in "most" cases.
(Whats the probability that a random unit vector has inner product epsilon with another unit vector?
Thats the probability of a false negative?)
### Analysis:
 finding k spanning n-dimensional vectors of an eigenspace can be done in O(knsC) time when the matrix is s-sparse where C is a stability constant I don't know at time of writing.
# Conclusion:
An n by n s-sparse matrix can have all of its eigenvectors approximately found in O(Csn^2) time !
(Hopefully, later, I can bound C, and the few other magic numbers still existing in this code.)

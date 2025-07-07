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
## Reduction 1:
The first reduction is well established relationship between the characteristic polynomial and eigenvalues.
We are reducing to the low spectral norm case for efficiency reasons.
```
eigenvalues(A):
  N = 2_norm_estimate(A)
  Beta = 2 // a number bigger than 1 which effects truncation error vs convergence error
  coefficients_polynomial = characteristic_polynomial_low_norm(A/N/Beta)
  return zeros_finding(coefficicents_polynomial)*N*Beta
```
### Stability Remark
If we need the eigenvalues to b bits of precision,
then we will need the coefficients to ```(b + log(K(A) + log(K(p(A) ) ) ) ``` bits of precision 
where this ```K(p(A))``` is the condition number of the characteristic polynomial of this modified A and
where this ```K(A)``` is the spectral condition number of the matrix.
Note: I do not know how these numbers are related.

## Reduction 2:
The second reduction is well established relationship of a polynomial's evaluation points and its coefficients.
We are choosing the sample points which let us use the fast fourier transform, and are also numerically stable.
```
characteristic_polynomial_low_norm(A):
  //ASSERT 2_norm(A) < 1/2
  x = list_roots_of_identity(n(A))
  sampled_polynomial = sampled_determinant(A,x )
  return inverse_fourier(sampled_polynomial) // if done correctly this is a fourier transform
```
### Stability Remark:
Because the inverse fourier transform is a scaled unitary matrix, we know the condition number of this step is ``` sqrt(n)```, 
and so if we need the answer with ```b ```bits of precision, we will need the subproblem with ```b + lg(n)/2``` bits of precision.
### Reduction 3:
The problem is to solve det(xI-A) for varying x.
We expand out the problem along the diagonal using a cheap formula for ```(I-e)^-1 = I + e +e^2 + e^3 ...```
The formula for expanding along the diagonal comes from the block matrix formula with 1 block being a 1 by 1.
```
sampled_determinant(A,x):
  dets = x hadmard_power n // might be better as just 1_n ?
  for i in 1 to n : // easily parallelized
    dets = dets hadmard_product ith_term_determinant(A,x,i) 
  return dets
ith_term_determinant(A,x,i):
  product = (column i up to row i-1 (A))
  initialize z to be empty
  for j in 1 to p:
    z_j = (row i up to column i-1(A)) * product
    product = A * product //truncating the matrix might speed things up but doesn't effect asymptotic runtime.
  y = Vandermonde_multiply(entrywise_inverse(x), z) //IF chosen correctly, this is a fourier transform but will need padding with a lot of zeros.
  return 1_n - (A_ii) * entrywise_inverse(x) - y
```
### Analysis
Work is O(nlogn + nlogn + n^2logn +  + psn^2) for the four layers of heavy logic.
Clearly as p -> infinity, the inverse formula holds.
Furthermore, if we need only need b bits of precision,
then roughly the (b+1)th term should be dropable with no loss of accuracy. (And we can drop all those following
## Analysis:
In total work should be bounded by O(sn^2(log(n) + log(K(A)) + log(K(p(A'))) ) ).
#### Counterexample Warning:
If either condition number is exponential in n ( and thus not a speedup over cubic methods),
you will need polynomial in n bits of precision for any standard solution.

# Eigenvector Problem:
## Reduction 1:
This first reduction follows from the definition of an eigenvector.
```
eigenvector(lambda,m,A):
  return nullspace(A- lambda * I_n,m)
```
## Reduction 2:
The second reduction is from nullspace to random sampling:
#### WARNING: If we need an orthonormal vector set within the same eigenspace that will be MORE work
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
  // So a greedy coloring on that graph with s^2 colors gets you the division.
  // A lazier bad division is all rows separately.
  for all i:
    let a_i be the row 2-norms of A_i
  v = c //start here
  i = 1
  //using epsilon and delta for precision.
  while 2-norm(A*v) > epsilon :
    v = v - (A_iT * ( Diag(entrywise_inverse(a_i hadmard a_i) ) (A_i * v)))
    normalization_constant = cT * v
    if abs( normalization_constant)  < epsilon   :
      throw NO VECTOR ERROR
    if vT * v > delta :
      throw NO VECTOR ERROR
    v = v * b / normalization_constant
    i++
    if i > number_of_orthogonal_groups :
      i = 1
  return v 
```
### Analysis & Concerns:
The argument which is most dubious is:
Is the random sampling sufficient to get the whole nullspace?
Other Concern:
Clearly, if a vector is returned its an approximate nullspace vector.
However, It might not be obvious to everyone why the algorithm converges in "most" cases.
Furthermore, We have not bounded the number of iterations needed to converge.
(Whats the probability that a random unit vector has inner product epsilon with another unit vector?
Thats the probability of a false negative from the normalization constant step.)
### Analysis:
Finding k spanning n-dimensional vectors of an eigenspace can be done in O(knsC) time when the matrix is s-sparse where C is a stability constant.
 
# Conclusion:
An n by n s-sparse matrix can have all of its eigenpairs approximately found in O(sn^2(C + b+log(nKP)) time 
where K is the matrix condition number and P is the other condition number, 
b is the precision you want eigenvalues with, and C is a yet to bound stability constant.



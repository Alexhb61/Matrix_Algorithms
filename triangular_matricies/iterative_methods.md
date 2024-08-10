# Extending Iterative Methods for Systems of linear equations.
## Intro
Say we have an iterative method for systems of linear equations.
Frequently those methods breaks the matrix into a Lower triangular, diagonal, and upper triangular part.
We could say that the method solves the equation Ax = b given subroutines for Triangular matrix-vector multiplication and back substitution and sometimes diagonal matrix-vector multiplication. 
The Gauss-seidel method for example works this way.
The Guass-seidel method is known to converge for positive definite matricies.
We want to extend it to say full rank matricies.
By viewing the method as a reduction, we can generalize the method by changing out the subroutines as necessary to get the convergence condition satisfied.
## Gauss Seidel Generalized
We want to solve ```Ax = b```.
We will apply Gauss-Seidel to ```A^TAx= A^Tb``` where ```A^T``` is the conjugate transpose of A
We will use a new routine for back substitution and multiplication of the implied gram matrix.
## Claim 1 : This will find the correct x
Let x be a solution to ```Ax = b``` then by associativity of matrix multiplication ```x``` solves ```A^TAx = A^Tb```.
Let x be a least squares solution to ```A^TAx = A^Tb```, then ```x = (A^TA)^+A^Tb``` by least square properties where ```M^+``` is the moore-penrose pseudo-inverse.
By the moore-penrose pseudo inverse property "reduction to the hermitian case",
then ```x = A^+b``` which is the least squares solution to ```Ax = b```.
## Claim 2 : This Method will converge.
Because ```A^TA``` is the gram matrix of A, it is positive definite if A is full rank.
## Claim 3: Strictly Upper Triangle of Gram matrix multiplication.
TMM(A,c) when A has >=2 columns:
Let A = [B D] 
Compute e = B^T(Dc)
output = [TMM(B, c) + e; TMM(D,c) ]
TMM(A,c) when A has 1 column:
ouput = 0
Work:  
``` T(k) <= 2*T(k/2) + k * n```
Then: O((n^2)log(n))
## Claim 4: Back Substitution of a Lower Triangle of a Gram matrix.
IT(A,c) when A has >= 2 columns
Let A = [B D] 
Let c = [e;f]
Let x = IT(B,e)
Let g = f - D^T(B(x))
let y = IT(D,g)
output = [x; y]
Work:
``` T(k) <= 2*T(k/2) + k * n ```
Then O((n^2)log(n))

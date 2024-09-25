# Extending Iterative Methods for Systems of linear equations.
#### Note: This paper will be broken into 1 or 2 conference papers soonish.
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
Because ```A^TA``` is the gram matrix of A, it is positive definite if A is full rank. (otherwise the gram matrix is positive semidefinite.)
## Claim 3: Strictly Upper Triangle of Gram matrix multiplication.
```
TMM(A,c) when A has >=2 columns:
Let A = [B D] 
Compute e = B^T(Dc)
output = [TMM(B, c) + e; TMM(D,c) ]
TMM(A,c) when A has 1 column:
ouput = 0
```
####  Work:  
``` T(k) <= 2*T(k/2) + k * n```
Then: ``` O((nm)log(n))``` arithmetic operations.
#### Depth:
If two recursive calls are done in parallel with eachother and the two matrix vector multiplications:
```T(k) <= max( T(k/2), log(n)) <= T(k/2) + log(n) ```
Then ```O(log(n)log(m))``` depth chain of arithmetic operations.

## Claim 4: Back Substitution of a Lower Triangle of a Gram matrix.
```
IT(A,c) when A has >= 2 columns
Let A = [B D] 
Let c = [e;f]
Let x = IT(B,e)
Let g = f - D^T(B(x))
let y = IT(D,g)
output = [x; y]
IT(A,c) when A has 1 column a:
output =  c/(a.a)
```
#### Work:
``` T(k) <= 2*T(k/2) + k * n ```
Then ```O((nm)log(n))```

#### Depth:
Notice that the two recursive calls are in series and can not be done in parallel. Likewise the two vector matrix multiplications are in this same series.
```T(k) <= 2T(k/2) + log(n) + log(k) ```
Then ```O(nlog(m))``` depth chain of arithmetic operations.

## Claim 5: Gauss Siedel can be stabilized.
Gauss Siedel Method starts with ```x_0 = 0``` (the zero vector)
and uses the recursive relation ```Lx_(i+1) = b - Ux_i ``` .
to solve ```Ax=b``` where ```A = L + U ``` and L is lower triangular and U is strictly upper triangular.
Using the subroutines above:
```
GS(A,b,C) 
x := 0
for i in 1 to C :
  x := IT(A,b-TMM(A,x) )
return x
```
where C is some convergence constant.
The Stabilized version can be written as :
```SGS(A,b,C)
return 2*GS(A,b,C) - GS(A,b,2*C) 
```
Most iterative methods can be seen as multiplying a vector by a geometric series of matricies.
The geometric series of matricies ``` I + A + A^2 + A^3 + A^4 ... ``` converges if the spectral radius is less than 1. [citation needs to be found]
However, when an eigenvalue = 1, this series goes to infinity.
In contrast, the equation based on the series ```2*s_n - s_2n = 0 ``` if ```s_n = n```. 
```2*s_n - s_2n -> L``` if the series ```s_n -> L``` converges.
Thus, the stabilized version converges for the eigenvalue 1 as well.
I believe when the ```A``` in ```Ax=b``` is positive semidefinite the eigenvalues of the iteration matix are real and less than one. [citation needs to be found]
Thus, Gauss siedel can be stabilized with a constant factor increase in work and depth
## Claim 6: Inverting a triangular matrix in low depth and nearly matrix multiplication work:
Inside the claim of inverting a matrix in low depth (NC complexity class) is neat facts about nilpotent matricies. [citation needs to be found]
Here is the nilpotent matrix fact we will need:
```(I - N)^(-1) = sum_(i=0 to n ) N^i```  where ```N^0 = I``` 
Note that N^n = 0 by definition of nilpotent.
we can use this to invert triangular matricies.
```T = D(I-N)``` where D is a diagonal matrix, I is the Identity matrix and N is strictly (lower) triangular.
```N_ij = - T_ij/T_ii``` is the equation we want.
```
Invert(T) :
Let N_1_ij = - T_ij/T_ii where i != j and all other entries of N_1 equal zero
for i = 2 to lg(n) :
  N_i = N_(i-1) * N_(i-1)
Let Inv_ii = 1/T_ii and all other entries of Inv equal zero
Let I be the identity
for i = 1 to lg(n) :
  Inv := Inv + N_i*Inv
return Inv
```
This is basically a lower work organization of a previously established result.
This is done in log(n) matrix multiplications in terms of both depth and work.
## Claim 7: Pseudoinverse in nearly matrix multiplication work
This basically writes out the matrix expansion implied by the stabilized gauss siedel method, but
does it with only lgC multiplications for Convergence constant C. It requires one call to the triangular inversion procedure.
```
PseudoInvert(S) : where S is a square positive semidefinite matrix.
Let C be a power of two and more than the number iterations needed for the stabilized gauss seidel method to converge.
Let L + U = S be the lower and strictly upper triangular matrix parts of the matrix S. 
Iter_1 = Invert(L)*U
for i = 2 to lg(C) + 1 :
  Iter_i = Iter_(i-1) * Iter_(i-1)
PInv = I + Iter_1
for i = 2 to lg(C) :
  PInv = PInv + PInv* Iter_i
PInv = PInv - PInv* Iter_(lg(C)+1) 
Return PInv
```
##  Another Idea: Mixing the two results
Another combination of the above results is to switch from using the back substitution implied in claim 4 to the inversion algorithm in claim 6.
This would let the work slide from mnklogn for variable k and the depth be (n/k)logm*polylogk this could be optimized to the machine.
Optimizing it for a machine is out of scope for my current skills.
## Conclusion : 
Solving a system of m linear equations in n variables can be done in O(mnlog(n)) with a hidden constant depending on the convergence rate of the gauss seidel method.

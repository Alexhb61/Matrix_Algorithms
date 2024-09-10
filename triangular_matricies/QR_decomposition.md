# QR Decomposition
QR decomposition is a fundamental linear algebra operation which I believe I can speed up.
I somewhat selfishly want to get cited on this wikipedia page : https://en.wikipedia.org/wiki/Computational_complexity_of_mathematical_operations
## The problem
Given any complex n by k matrix A where n >= k: 
We seek a pair of matricies Q,R such that
1. R is upper triangular k by k. For all i,j in [k] : if i > j then R_ij = 0
2. Q is rectangular unitary n by k. Q^T = Q^+ The conjugate transpose is equal to the moore-penrose psuedoinverese.
3.  A = QR . IE this is a decomposition of matrix A in particular.
## Analyzing The Gram Schmit Process
The Gram-Schmit process is an old algorithm for finding the orthonormal basis of a matrix which is equivalent to the Q in a QR decomposition. It finds one orthogonal vector at a time by subtracting off the projections of that column onto precomputed orthogonal vectors. A bad implementation of it will take O(nk^2) work and O(k^2(logn)) depth. A better implementation of it will take O(klogkn) depth.
Here is some pseudocode for the orthonormalization.
```
GS(A):
for i in 1 to k:
u_i = a_i - sum j=0 to i-1 proj(q_i-j,a_i) 
q_i = u_i/||u_i||
return [q_1...q_k]
```
It is possible to extract the triangular matrix by keeping track of the inner products.
There are three concerns I have with this algorithm:
1. It is not numerically stable [CITATION NEEDS FINDING]
2. It is of significant depth
3. It ignores or preceeds fast matrix multiiplication methods.
## Reorganizing the Gram Schmit Process
By writing it in matrix form, we can find speedups, but correctness needs to be shown.
``` q_i = c_i - Q_i-1 (Q_i-1)^Tc_i ```
By noting that using the conjugate transpose:
``` U = (I - QQ^T)B ``` for rectangular-unitary Q causes U to span the space of B not in Q, but is orthogonal to Q.
```Q^TU = Q^T(I-QQ^T)B = (Q^T-Q^TQQ^T)B = (Q^T-Q^T)B = 0``` orthogonal property.
We can construct a reorganization of the Gram Schmit Process which uses more matrix matrix multiplication than matrix vector multiplications which depending on implementation detail might give speedups:
### Algorithm Reorganized GramSchmit process
```
RGSP(A):
If A has 1 column a:
  r = sqrt( a dot a)
  return ( [a/r] , [r] )
Let A = [BC] where B and C are roughly half the columns of A
(U,T) = RGPSP(B)
S = U^TC
D = C - US
(V,R) = RGSP(D)
return ([U V] [T S; 0 R]
```
This algorithm has numerical stability issues both because its based directly on Gram-Schmit, 
and also because it might divide by zero.
### Claim 1: Up to numerical stability problems, RGSP computes a decomposition:
Base case clearly a == [a/r]*[r]
inductive case:
```[U V] * [T S; 0 R] ```
do block matrix multiplication
```=[U*T + V*0   U*S + V*R]```
apply inductive hypothesis for U,T and V,R
```= [B +V*0  U*S + D]```
apply domination property of zero, and undo definition of D
```= [B + 0  U*S + C - U*S]```
apply identity properties of matrix addition:
```=[B C]```
Thus the algorithm computes a decomposition up to numerical stability problems (addition not being associative, division by zero)
### Claim 2: RGSP returns an rectangular unitary matrix. ie Q^T = Q^+
#### Subclaim A: we only need to show that Q^T and Q are weak inverses of eachother since the other two moore-penrose conditions are trivial for a matrix and its conjugate transpose.
#### Subclaim B: assuming the algorithm doesn't divide by zero, all we need to show is Q^TQ = Ik
if Q^TQ = Ik then QQ^TQ = Q and Q^TQQ^T = Q^T
Base Case: ```[a/r]^T*[a/r] = [1]``` because r is the norm of a.
Inductive case: ```[U V]^T * [U V]``` multiplyin it out gives
```= [U^TU U^TV ; V^TU V^TV ]``` which by the inductive hypothesis becomes
```= [I U^TV; V^TU I ] ``` now all we need to show is the off diagonal squares are zero.
```U^TV = U^TDR^-1``` by decomopsition property
```=U^T(C-US)R^-1``` by substitution of definitions
```=U^T(C-UU^TC)R^-1``` by substitution of definitions
```=(U^TC - U^TUU^TC)R^-1``` by distribution
```= (U^TC-U^TC)R^-1``` by inductive hypothesis
```= 0``` by identity properties of matrix ring
### Claim 3: RGSP returns an upper triangular matrix
Trivial base case. Inductive cases follow from interaction of upper triangular and block matrix facts.
### Claim 4: RGSP runs in depth O(k)
Its depth recursion relation is
```T(k) = 2T(k/2) + O(log(nk) ); T(1) = log(n)```
when assuming matrix multiplies happen in log depth.
Applying the master theorem for recursive relations gets the bound.
Note that while a divide and conquer relation, it is a one part second part algorithm, so it doesn't more parallel.
### Claim 5: RGSP runs in work O(nk^(w-1)) where w is the exponent for fast matrix multiplication.
The work recursion relation is
```T(k) = 2T(k/2) + O(nk^w-1); T(1) = O(n)```
Applying the master theorem for the recursive bound shows that while w > 2 the claimed runtime holds.
For w=2, some log factors show up both in the matrix multiplication work and the final recursive bound.
## Analyzing The Modified Gram Schmit Process

## Reorganizing the modfied Gram Schmit Process
## Top Down Method
## Bottom Up Method

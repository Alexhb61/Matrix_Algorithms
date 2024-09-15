# QR Decomposition
QR decomposition is a fundamental linear algebra operation which I believe I can speed up.
I somewhat selfishly want to get cited on this wikipedia page : https://en.wikipedia.org/wiki/Computational_complexity_of_mathematical_operations
## The problem
Given any complex n by k matrix A where n >= k: 
We seek a pair of matricies Q,R such that
1. R is upper triangular k by k. Meaning: For all i,j in [k] : if i > j then R_ij = 0
2. Q is rectangular unitary n by k. Meaning: Q^T = Q^+ i.e. The conjugate transpose is equal to the moore-penrose psuedoinverese.
3.  A = QR . i.e. this is a decomposition of matrix A in particular.
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
We will prove this recursive algorithm to have this output property by induction on k.
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
#### Subclaim C:Q^TQ = Ik for all input sizes k
We will proved this recusive algorithm to have this output property via a proof by induction on k.
Base Case: ```[a/r]^T*[a/r] = [1]``` because r is the norm of a.
Inductive case: ```[U V]^T * [U V]``` multiplying it out gives
```= [U^TU U^TV ; V^TU V^TV ]``` which by the inductive hypothesis becomes
```= [I U^TV; V^TU I ] ``` now all we need to show is the off diagonal squares are zero.
```U^TV = U^TDR^-1``` by decomopsition property on D
```=U^T(C-US)R^-1``` by substitution of definition of D
```=U^T(C-UU^TC)R^-1``` by substitution of definition of S
```=(U^TC - U^TUU^TC)R^-1``` by distribution property of the matrix ring
```= (U^TC-U^TC)R^-1``` by the inductive hypothesis
```= 0``` by identity properties of matrix ring
Thus, ```[U V]^T[U V] = Ik``` as intended.
### Claim 3: RGSP returns an upper triangular matrix
Trivial base case. Inductive cases follow from interaction of upper triangular and block matrix facts.
### Claim 4: RGSP runs in depth O(k)
Its depth recursion relation is
```T(k) = 2T(k/2) + O(log(nk) ); T(1) = log(n)```
when assuming matrix multiplies happen in log depth.
Applying the master theorem for recursive relations gets the bound.
Note that while a divide and conquer relation, it does one part THEN a second part, so it doesn't parallelize well.
### Claim 5: RGSP runs in work O(nk^(w-1)) where w is the exponent for fast matrix multiplication.
The work recursion relation is
```T(k) = 2T(k/2) + O(nk^w-1); T(1) = O(n)```
Applying the master theorem for the recursive bound shows that while w > 2 the claimed runtime holds.
For w=2, some log factors show up both in the matrix multiplication work and the final recursive bound.
### Claim 6: Up to associativity and numerical errors, this is still the Gram-schmit process.
All that has changed is when we subtract off the projections so that we can use fast matrix multiplication.
This acts a really nice distinction between mathematics and algorithms 
in that how you arrange the math effects how long and stable the math is.
Because of this, I suspect that the numerical instability of the Gram Schmitt Process still persists.
So This method improves upon one of the three properties mentioned above (it benefits from fast matrix mulitiplication) but it isn't much better than that.
## Analyzing The Modified Gram Schmit Process
In order to deal with numerical instability of the original Gram Schmitt Process, 
a different method was made by [FIND AUTHOR(s)]
It uses the fact that (I -uu^T) is a more effective projection when used in series rather than in parallel.
A poor implementation of it will take O(k^2logn) depth, but better implementations exist such as below that take O(klognk)
```
MGS(A) :
if A is a vector a :
  r = sqrt(a dot a)
  return ( [a/r], [r] )
let A = [a B] where a is a vector and B is the remaining columns of A
r = sqrt( a dot a )
q = a / r
s = q^TB #note this is a row vector not a column vector
C = B - qs
(Q,R) = MGS(C)
return  ( [q Q] , [ r s ; 0 R ] )
```
This method is more stable [citation needed]
## Reorganizing the modfied Gram Schmit Process
Again, we can reorganize this slightly more stable algorithm to take advantage of faster matrix matrix mulitplications. (note that might ruin the numerical stability)
The reorganization needs to keep track of a third thing the orthogonal projection Product i=1 to k (I-u_iu_i^T) ...
We will do this by constructing a matrix (I-QSQ^T) = the product before. 
### Algorithm Reorganized Modified Gram Schmitt Process
```
RMGSP(A) :
if A is a vector a :
  r = sqrt( a dot a )
  return ( [a/r], [r], [1] )
let A = [B C] where B, C are matricies of the roughly half the columns.
(Q_1, R_1, S_1) = RGMSP(B)
N = S_1Q_1^TC
D = C - Q_1N
(Q_2, R_2, S_2) = RGMSP(D)
S_3 = (S_1)(Q_1^T)(Q_2)(S_3)
return ( [Q_1 Q_2] , [R_1 N; 0 R_2] , [S_1 -S_3 ; 0 S_2 ] )
```
### Claim 1: This Algorithm RMGSP computes a decomposition:
We will prove this decomposition fact by strong induction on the input width k.
Base Case: ```a = [a/r]*[r]``` 
Inductive Case: ``` [Q_1 Q_2] * [R_1 N ; 0 R_2] ```
multipling it out: ```[ Q_1*R_1 + Q_2*0  Q_1*N + Q_2*R_2 ]```
applying inductive hypothesis on both calls ```=[B+Q_2*0  Q_1*N + D] ```
substituting for the definition of D : ```=[B +Q_2*0 Q_1*N + C - Q_1*N ]```
Applying simple properties of the matrix ring: ```=[B C] ```
### Claim 2: This algorithm RMGSP computes an upper triangular matrix R when outputing (Q,R,S)
We can prove this by strong induction on the input width k.
Base case trivial.
Inductive case follows from a block decomposition of an upper triangular matrix.
### Claim 3: This algorithm RMGSP computes the serial orthogonal projection P = (I -QSQ^T) when outputing (Q, R, S)
We will prove this by strong induction.
Base case:
 ```(I - qsq^T) =  (I-qq^T)``` because ```s = [1]``` in the base case.
Inductive case:
```(I - QSQ^T) = I - Q_1S_1Q_1^T -Q_2S_2Q_2^T +Q_1S_3Q_2^T ``` by multiplying out the definitions of S and Q
```= I - Q_1S_1Q_1^T - Q_2S_2Q_2^T + Q_1_S_1Q_1^TQ_2S_2Q_2^T``` by expanding the definition of S3
```=(I-Q_1S_1Q_1^T)(I-Q_2S_2Q_2^T)``` by factoring out the terms
Which by the inductive hypothesis applied to the two subroutine calls equals the serial orthogonal projection.
### Claim 4: ??? Q is rectangular unitary
NUMERICAL STABILITY ANALYSIS NEEDED
I need a Lemma relating the serial orthogonal projection to rectangular unitary matrix....
### Claim 5: RMGSP runs in O(k) depth arithmetic operations
The recursion relation is: ```T(k) <= 2T(k/2) + log(kn) ; T(1) = log(n) ``` like before and so takes O(k) depth.
DOES IT TAKE O(klogn) depth because the log(n) term acts as a very large constant?
### Claim 6: RMGSP runs in O(nk^(w-1) ) work
Same argument as above...
## Top Down Method
Was previously inside SVD paper.

## Bottom Up Method

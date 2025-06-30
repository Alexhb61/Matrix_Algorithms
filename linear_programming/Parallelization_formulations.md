# Introduction:
This is a writeup of multiple paths from Fetch method & 2-Norm Oracle & constrained Oracle to NC= P.

# Stable Programs
## H(A) inner product Lemma:
Let A have 2-norm-normalized rows, and let the inner product of any 2 rows be non-negative.
Then the H(A) for 2-norm is 1 .
Proof: ||x-x_opt||^2 <= ||(Ax-b)+||^2
because no obtuse kite effects occur only squares or acute kite effects occur.
#### HOW TO MAKE THIS MORE RIGOUROUS
## ||A|| sparsity lemma:
Let A have 2-normalized rows, and let A have at most s nonzero elements per row.
Let A have at most z elements per column.
Then the 2-norm of A is at most sqrt(sqrt(s)z).
Proof: 
```2-norm(A)^2 <= 1-norm(A)*infinity-norm(A) <= sqrt(s)*z``` and now the bound holds.
## sparse and well conditioned program.
Given a monotone circuit on G gates, we can construct a sparse linear program on O(G) variables.
Note that each variable of a gate should only occur 1 outside its definition. (the time when its used).
### Input a used k times
```a_1 <= 1``` iff on ```a_1 <= 0``` iff off
```a_i - a_(i-1) <= 0``` (k-1 times i ranges from 2 to k) 
### a = b OR c used k times
```a_0 - b - c <= 0 ```
```a_0 <= 1```
```a_i - a_(i-1) <= 0``` (k times i ranges from 1 to k) 
### a = b AND c used k times
```a_0 - b <= 0```
```a_0 - c <= 0```
```a_i - a_(i-1) >= 0``` (k times i ranges from 1 to k) 
### GOAL
maximize sum of outputs
### Sparsity:
we have 3 uses per variable and 3 entries per constraint.
### Making a sparse program well conditioned:
For any pair of constraints whose inner product is negative,
we add a dummy variable d to both constraints, 
and a new constraint d <= 0
The number of new variables is at most s^2 n.
The number of uses per variable is at most max(2,s)
The number of entries per constraint is at most s^2 + s.
So the new program is still sparse.
#### Finally,
We need to force all the dummy variables to be zero.
We can do that with one equality constraint :
sum dummy = 0.
## Conclusion:
If this program is as stable as I believe, it existing shows NC= P

# Separated Programs:
Given a Circuit of AND and OR gates, we can write it as a linear program 
with each gate being its own block in the matrix and no-overlaps.
Then connect the gates with equality constraints considered separately.
### AND Gates:
```a_i - b_i <= 0 ```
```a_i - c_i <= 0```
```-a_i + b_i +c_i <= 1```
### OR gates:
```-a_i + b_i <= 0```
```-a_i + c_i <= 0```
``` a_i -b_i -c_i <= 0 ```
## Block Bounds
The 2-norm of a block matrix is at most the 2-norm of any block. Proof: follows from spectral definition.
The 2-norm based hoffman constant for a block matrix is at most the hoffman constant of any block.
Proof: follows from spectral definition of submatrix.
## Fast inverse of the equality constraints:
Each value in the circuit will have its own set of variables which all need to be set equal.
We can easily compute the pseudoinverse, as the pseudoinverse of a block matrix is just the pseudoinverses of the blocks separately.
Furthermore the constraints to set k values to the same value can be done
via a k by k matrix with 1's down the main diagonal and -1's down the second circulant. 
This circulant matrix is easily put into diagonal form via the fourier transform,
and thus easily pseudoinverted in fast parallel time.
## Concern:
Did I correctly analyze that the constant D is unaffected by adding constraints?

# Convex Dual Program
We want to solve ```Ax<= b```
by looking for a ```y>= 0 ; yTAATy <= 1; max (Ax-b)Ty ```
we can break ```AAT = UDDUT``` where D is diagonal and U is orthogonal.
Then break into:
```Ux >= 0; Dx = z ; zTz <= 1 ```
This can be solved quickly because U has condition number 1 and norm 1.
Similarly the ```zTz <= 1``` constraint will just have similarly distance = epsilon situation.
So? does this succeed in one iteration? Maybe? If the approximation is any constant, this works as a NC/poly = P/poly algorithm.

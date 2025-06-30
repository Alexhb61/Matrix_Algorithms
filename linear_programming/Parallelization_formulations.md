# Introduction:
Multiple paths from Fetch method to NC= P.

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
# Convex dual Program:
We can solve any linear program Ax<= b if given a starting point x_0 
and a bound R on the distance between x_0 and the polytope of the program
by solving the problem of finding the distance to the polytope from x_0.
## The program:
This dual problem has the form:
```y >= 0 ```
```yTAATy <= 1```
```max (Ax_0-b)Ty ```
Essentially we are trying to construct the most violated constraint possible.
Then ```x_true = x_0 - ATy*max_value```.
If we limit y to only the terms that have nonzero value to maximize,
then the program is feasible whenever the remaining matrix has full rank.
However, I don't believe the converse holds.
This program has the implicit constraints that  ```y_i <= 2-norm(row_i(A)) ```
## Stability:
I believe this program is stable because it is the intesection of a box and ellipsoid,
and so should lack the pointy parts which make other programs unstable.
Although, I might be imagining a axis-aligned elliposid which may not generalize to other ellipsoids.
## Solving:
We can solve this program with log(R/r) calls to the fetch method 
where we binary search on the optimum of this program, 
and make the optimization criteria an equality constraint.
Lastly, we can turn the remaining two constraint sections into an oracle
by using the complex orcale described above on the matrix and vector:
```A = (-I_n) row_concat (yAAT) ; b = 0_n row_concat 1 ```
## Derivation of other program:
```y >= 0 ```
```yTAATy <= 1```
```max (Ax_0-b)Ty ```
limit A to the rows where (Ax_0-b) > 0 to be B.
Then singular value decompose ```BBT = USU*```
``` Ux >= 0 ```
``` xTSx <=1 ```
``` max (Ax_0-b)+Ux ```
We make a very good orcale for ```Ux>= 0``` via the complex orcale above.
Then, we constrain it to have a constant cx= d constraint from the maximzation criteria.
Finally, we ?alternate it? with the xTSx <= 1 constraints...

# Question: How does one combine two distance orcales?
A separation oracle for C intersect K can be easily constructed from a separation oracle for C and from a separation oracle for K.
However a distance oracle for C intersect K is harder to construct from the distance orcales, because the yes-yes case does not imply a yes case.


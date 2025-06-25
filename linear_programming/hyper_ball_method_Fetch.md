# Introduction
This is a second attempt at writing up simplifications and runtime improvements to the ellipsoid method.

The most interesting theoretical feature of the Ellipsoid Methods are their runtime being independent of the number of constraints.
Their most dissapointing practical feature is their numerical instability.
Their weirdest feature is that they solve problems using a separation orcale.

The Fetch Method is even weirder because it uses a distance orcale.
The Fetch Method is more practical because it appears to be numerically stable and elegant.
The Fetch Method is more theoretically interesting because its runtime is only strongly dependent on the numerical stability of the problem because it has logarithmic depth in terms of problem size.
## C-D-Distance Orcale:
A C-D distance orcale for a convex set K is an orcale which answers the question:
Is x within euclidean distance R to K? with one of the four answers:
1. No, x is not within distance R to K.
2. Maybe, but for all y in K, ```dy + eps <= dx``` 
  where ```eps <= R/C``` and ```eps >= R/D``` and ```||d|| = 1```
3. yes, x is distance at most d from K.```d < R ```
4. absolutely, x is in K.
# Fetch Method:
Given a C-D-distance Orcale for K within R^n, a starting sphere S of radius R, and a minimum radius r of a sphere within K,
the fetch method finds a point in K or rejects the premise of K intersect S containing a radius r sphere in good time.
## Correctness Idea
The Correctness of the method follows directly from this idea:
A hyperball intersected with a half-space is contained in a smaller hyperball whenever the center is not in the intersection. Applying the pythagorean theorem.
```||y-new_center||^2 <= ||y-old_center||^2 - ||new_center - old_center||^2 <= R^2 - epsilon^2```
## Method
```
Fetch(Orcale,x_0,R,r):
  x = x_0 // this where the dimension of the problem is grabbed
  radius_squared = R*R
  while radius_squared >= r*r :
    (status,direction, distance_bound) = Orcale(x, sqrt(radius_squared) )
    if status = absolutely :
      return (true, x)
    if status = yes :
      radius_squared = distance_bound*distance_bound
    if status = maybe:
      x = x - direction * distance_bound
      radius_squared = radius_squared - distance_bound*distance_bound
    if status = no :
      return (false, _)
  return (false, _)
```
## Analysis:
Each iteration of the while loop takes O(n) work and approximately log(n) depth.
Each iteration shrinks the radius squared by at least 
```(R^2-eps^2)/R^2 <= (1-(eps/R)^2) <= (1-1/D^2)``` multiplicatively.
So the iteration count is worst case ``` 2ln(R/r) *D^2``` 
since for sufficiently large n, ```(1-1/n)^n ```is roughly 1/e.
Total depth ```O(log(R/r)*D^2*(log(n) + orcale_depth) )``` which I believe is fast.
Total work ```O(log(R/r)*D^2*(n+orcale_work))```

# Simple 1-H(A')-Distance Orcale for a linear program.
This orcale is simple to describe and implement.
It essentially selects the most violated constraint to improve on.
We are solving the problem Ax<= b.
However, because it uses a mixed norm constant 
(the constant which bounds 2-norm of error in x given a infinity-norm of error in positive part of (Ax-b) after scaling each row by its norm...)
for C, it will likely be slower than optimal.
## Orcale
```
Orcale(A,b,x,r) :
  C = 1
  D = Weird_HOFFMAN_CONSTANT(A) // the constant which makes it a C-D-Orcale
  //in practice the constant can be set to infinity or doubled in size each time.
  error = elementwise_max(0,Ax-b) //easy to make parallel
  if error == 0 :
    return (absolutely,_,_)
  epsilon = 0
  for i in 1 to m: //possible to make parallel
      length = 2-norm(A_i)
      if error_i/length > epsilon :
        epsilon = error_i / length //element i of error vector scaled
        direction = A_i / length // row i of A scaled
  if r < C*epsilon :
    return (no,_,_ )
  if r > D* epsilon :
    return (yes,_, D* epsilon )
  return (maybe, direction, epsilon)
```
## Analysis
The normalization of the rows and epsilon makes certain that d is unit length.
C being one follows from the fact that projecting the vector (x-x_opt) onto a direction 
always shrinks the length of the vector.
The runtime for this orcale is O(nm) work and O(log(m) + log(n)) depth, because its dominated by the matrix operations.
### Weird D:
D is funkier, but to the best of my understanding I have described the appropriate hoffman constant.
We want the constant D such that ||x-x_best||_2 <= D ||(Ax-b)+||_infinity .
# Complex 1-H(A)*||A||-distance Orcale
This orcale is slightly more complicated because it generates a constraint to update.
## Orcale
```
Orcale(A,b,x,r):
  C = 1
  D = Hoffman_Constant_2-norm(A)*2-norm(A)
  //in practice the constant can be set to infinity or doubled in size each time.
  error = elementwise_max(0,Ax-b)
  if error == 0 :
    return (absolutely,_,_) 
  unnormalized_direction = A^t*error
  length = 2-norm(unnormalized_direction)
  direction = unnormalized_d/length
  epsilon = error^t*error/length
  if epsilon*D < r :
    return (yes,_,epsilon*D)
  if epsilon*C > r :
    return (no,_,_)
  return (maybe,direction, epsilon)
```
## Analysis:
C being one holds for the same reason as before.
This orcale's work is again bounded by O(nm) and again depth O(log(n)+log(m)).
### Noting D:
D is independent of scaling A and b, so the equation looking like condition number makes sense.
```||x-x_opt|| >= H(A)|*||(Ax-b)+| ``` first bound
```||(Ax-b)+||^2 = error^terror >= epsilon*length >= epsilon*2-norm(A^t)*2-norm(error) ``` 2nd bound
and thus, ```||(Ax-b)+|| >= epsilon*2-norm(A^t) ```
And finally, ```D = H(A)*2-norm(A^t)``` 
# Constrained Orcale:
This takes an orcale for a problem and adds an equality constraint. (cx = d)
## Pseudocode:
```
ConstrainedO(Orcale,c,d,x,r):
  (status,direction,distance_bound) = Orcale(x,r)
  if status == no :
    return (status,direction,distance_bound)
  if status == maybe:
    new_direction = direction - project direction onto c
    length = 2-norm(new_direction)
    new_direction /= length
    delta = (new_direction dot product direction)
    return (maybe, new_direction, distance_bound/delta )
  if status == yes
    (status,direction,distance_bound) = Orcale(x,distance_bound)
    assert(status == maybe)
    new_direction = direction - projection direction onto c
    length = 2-norm(new_direction)
    new_direction /= length
    new_distance_bound = distance_bound /(direction dot product new_direction)
    if new_distance_bound < r :
      return (yes,_, new_distance_bound )
    else :
      return (maybe, new_direction, new_distance_bound) 
  if status == absolutely :
    if cx = d :
      return (status,direction,distance_bound)
    else :
      throw error // initial x didn't satisfy constraint OR numerical instability problem
```
## Correctness and runtime Analysis:
It uses at most 2 runs of the inner orcale, and O(n) work and O(log(n)) depth.
I believe it doesn't effect D because epsilon only grows, but doesn't definitely grow.
The geometry seems clear.
## Concern Numerical Stability and C constant:
I can imagine a situation where the polytope defined by the inequalities is non-empty, but the polytope after intersecting with the equality constraint is empty. This could lead to a nearly zero new_direction which might then cause oscillation or other problems.
However, it at least means that 1/C is unbounded in that case.

# Stable Programs
## H(A) inner product Lemma:
Let A have 2-norm-normalized rows, and let the inner product of any 2 rows be non-negative.
Then the H(A) for 2-norm is 1 .
Proof: ||x-x_opt||^2 >= ||(Ax-b)+||^2
because no obtuse kite effects occur only squares or acute kite effects occur.
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

# Conclusion:
For a well behaved system of linear inequalitites, the fetch method with one of the orcales uses
depth ```O(log(n)*D^2*log(R/r))``` and work ```O(nmD^2*log(R/r))```.
With sufficient preconditioning, this algorithm can solve linear programming in polylogarithmic depth.
However the sufficient preconditioning to get to a NC = P result might be incorrect.

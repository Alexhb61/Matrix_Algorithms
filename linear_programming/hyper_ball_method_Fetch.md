# Introduction
This is a second attempt at writing up simplifications and runtime improvements to the ellipsoid method.

The most interesting theoretical feature of the Ellipsoid Methods are their runtime being independent of the number of constraints.
Their most dissapointing practical feature is their numerical instability.
Their weirdest feature is that they solve problems using a separation orcale.

The Fetch Method is even weirder because it uses an approximate distance orcale.
The Fetch Method is more practical because it appears to be numerically stable and elegant.
The Fetch Method is more theoretically interesting because its runtime is only strongly dependent on the numerical stability of the problem because it has logarithmic depth in terms of problem size.
## D-approximate-Distance Orcale:
A D distance orcale for a convex set K is an orcale which answers the question:
Is x within euclidean distance R to K? with one of the four answers:
1. No, x is not within distance R to K.
2. Maybe, but for all y in K, ```dy + eps <= dx``` 
  where ```eps <= R``` and ```eps >= R/D``` and ```||d|| = 1```
3. yes, x is distance at most d from K.```d < R ```
4. absolutely, x is in K.
# Fetch Method:
Given a D-distance Orcale for K within R^n, a starting sphere S of radius R, and a minimum radius r of a sphere within K,
the fetch method finds a point in K or rejects the premise of K intersect S containing a radius r sphere in good time.
## Correctness Idea
The Correctness of the method follows directly from this idea:
A hyperball intersected with a half-space is contained in a smaller hyperball whenever the center is not in the intersection. Applying the pythagorean theorem in the most extreme case.
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

# Simple H(A')-approximate-Distance Orcale for a linear program.
This orcale is simple to describe and implement.
It essentially selects the most violated constraint to improve on.
We are solving the problem Ax<= b.
However, because it uses a mixed norm constant 
(the constant which bounds 2-norm of error in x given a infinity-norm of error in positive part of (Ax-b) after scaling each row by its norm...) it will likely be slower than optimal.
## Orcale
```
Orcale(A,b,x,r) :
  D = Weird_HOFFMAN_CONSTANT(A) // the constant which makes it a D-Orcale
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
  if r < epsilon :
    return (no,_,_ )
  if r > D* epsilon :
    return (yes,_, D* epsilon )
  return (maybe, direction, epsilon)
```
## Analysis
The normalization of the rows and epsilon makes certain that d is unit length.
The runtime for this orcale is O(nm) work and O(log(m) + log(n)) depth, because its dominated by the matrix operations.
### Weird D:
D is funkier, but to the best of my understanding I have described the appropriate hoffman constant.
We want the constant D such that ||x-x_best||_2 <= D ||(Ax-b)+||_infinity .
This D is bounded above by sqrt(n) times the infinity norm version of the hoffman constant,
and is also bounded above by sqrt(m) times the 2-norm version of the hoffman constant.

Furthermore, there seem to be a way to bound the hoffman constant 
in terms of a maximum over all full row rank submatricies, the matrix norm of the inverse.
Here is a link to a hoffman constant computation: https://arxiv.org/abs/1804.08418


# Complex H(A)*||A||-distance Orcale
This orcale is slightly more complicated because it generates a constraint to update.
## Orcale
```
Orcale(A,b,x,r):
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
  if epsilon > r :
    return (no,_,_)
  return (maybe,direction, epsilon)
```
## Analysis:
This orcale's work is again bounded by O(nm) and again depth O(log(n)+log(m)).
### Noting D:
D is independent of scaling A and b, so the equation looking like condition number makes sense.
```||x-x_opt|| <= H(A)|*||(Ax-b)+| ``` first bound
```||(Ax-b)+||^2 = error^terror = epsilon*length <= epsilon*2-norm(A^t)*2-norm(error) ``` 2nd bound
and thus, ```||(Ax-b)+|| <= epsilon*2-norm(A^t) ```
And finally, ```D = H(A)*2-norm(A^t)``` 
# Constrained Orcale:
This takes an orcale for a problem and adds a matrix of equality constraints (Cx = d)
## Pseudocode:
```
ConstrainedO(Orcale,C,d,x,r):
  (status,direction,distance_bound) = Orcale(x,r)
  if status == no :
    return (status,direction,distance_bound)
  if status == maybe:
    new_direction = direction - (CT(CCT)^+C)direction
    // where the plus superscript is moore penrose pseudoinverse and the T is transpose.
    length = 2-norm(new_direction)
    new_direction /= length
    delta = (new_direction dot product direction)
    distance_bound /= delta
    if distance_bound < r :
      return (maybe, new_direction, distance_bound )
    else :
      return (no,_,_)
  if status == yes
    (status,direction,distance_bound) = Orcale(x,distance_bound)
    assert(status == maybe)
    new_direction = direction - (CT(CCT)^+C)direction
    length = 2-norm(new_direction)
    new_direction /= length
    new_distance_bound = distance_bound /(direction dot product new_direction)
    if new_distance_bound > r :
      return (no,_,_)
    else if new_distance_bound*D < r
      return (yes,_, new_distance_bound*D )
    else :
      return (maybe, new_direction, new_distance_bound) 
  if status == absolutely :
    if Cx = d :
      return (status,direction,distance_bound)
    else :
      throw error // initial x didn't satisfy constraint OR numerical instability problem
```
## Correctness and runtime Analysis:
The Computational Complexity of the pseudoinverse and the matrix vector multiplications vary in significance.
It uses at most 2 runs of the inner orcale, and O(n) work and O(log(n)) depth.
I believe it doesn't effect D because epsilon only grows, but doesn't definitely grow.
The geometry seems clear.

## Concern Numerical Stability:
I can imagine a situation where the polytope defined by the inequalities is non-empty, but the polytope after intersecting with the equality constraint is empty. This could lead to a nearly zero new_direction which might then cause oscillation or other problems.

# Conclusion:
For a well behaved system of linear inequalitites, the fetch method with one of the orcales uses
depth ```O(log(n)*D^2*log(R/r))``` and work ```O(nmD^2*log(R/r))```.
With sufficient preconditioning, this algorithm can solve linear programming in polylogarithmic depth.
However the sufficient preconditioning to get to a NC = P result might be infeasible.

## Warning:
Condition numbers can easily be exponential in n,
and Hoffman constants are similarly unstable.
This algorithm has quadratic runtime in terms of these constants.
Even if I assume that the matrix is sparse integers,
the best upper bound I've found so far is exponential in n (not m).


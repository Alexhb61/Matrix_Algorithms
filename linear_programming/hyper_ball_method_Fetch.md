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
Is x within euclidean distance R to K? with one of the three answers:
1. No, x is not within distance R to K.
2. Maybe, but for all y in K, dy + eps < dx 
  where eps <= R/C and eps >= R/D and ||d|| = 1
3. yes, x is distance at most d from K. d < R 
# Fetch Method:
Given a C-D-distance Orcale for K within R^n, a starting sphere S of radius R, and a minimum radius r of a sphere within K,
the fetch method finds a point in K or rejects the premise of K intersect S containing a radius r sphere in good time.
The Correctness of the method follows directly from this idea:
A hyperball intersected with a half-space is contained in a smaller hyperball whenever the center is not in the intersection.
## Method
```
Fetch(Orcale,x_0,R,r,n):
  x = x_0
  radius = R
  while radius > r :
    (status,direction, distance_bound) = Orcale(x, radius)
    if status = yes :
      radius = distance_bound
    if status = no :
      return (false, _)
    x = x - direction * distance_bound
    radius = sqrt( radius^2 - epsilon^2 )
  if radius is nearly 0 :
    return (true, x)  
  return (false, _)
```
## Analysis:
Each iteration of the while loop takes O(n) work and approximately log(n) depth.
Each iteration shrinks the radius squared by at least 
```(R^2-eps^2)/R^2 <= (1-(eps/R)^2) <= (1-1/D^2)``` multiplicatively.
So the iteration count is worst case ``` 2ln(R/r) *D^2``` 
since for sufficiently large n, ```(1-1/n)^n ```is roughly 1/e.
Total depth ```O(log(R/r)*D^2*(log(n) + orcale_depth)``` which I believe is fast.
Total work ```O(log(R/r)*D^2*(n+orcale_work))```

# Simple 1-H(A)-Distance Orcale for a linear program.
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
  return (maybe, d, epsilon)
```
## Analysis
The runtime for this orcale is O(nm) work and O(log(m) + log(n)) depth, because its dominated by the matrix operations.
# Complex C-D-distance Orcale
This orcale is slightly more complicated because it generates a constraint to update.
For this algorithm, C is the reciprical of the 2-norm of A and D is the hoffman constant(A,b) which uses the 2-norm both times.
## Orcale
```
Orcale(A,b,x,r):
  C = 1/2-matrix-norm(A)
  D = Hoffman_Constant_2-norm(A)
  //in practice the constant can be set to infinity or doubled in size each time.
  error = elementwise_max(0,Ax-b)
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
This orcale's work is again bounded by O(nm) and again depth O(log(n)+log(m)).
# Conclusion:
For a well behaved system of linear inequalitites, the fetch method with one of the orcales uses
depth ```O(log(n)*D^2*log(R/r))``` and work ```O(nmD^2*log(R/r))```.
With sufficient preconditioning, this algorithm can solve linear programming in polylogarithmic depth.
However the sufficient preconditioning to get to a NC = P result might be impossible.

# A Simplified ellipsoid Method: the hyperball method
# Introduction
So the ellipsoid method is a famous method for solving linear programs and convex programs in general.
The method and its variants have two nice properties:
1. Their runtime is independent of the number of constraints because they access the problem via an orcale.
2. Their runtime is polynomial in the amount of precision needed.
The orcale that the original method uses is a separation orcale:

#### Separation Orcale for convex set K:
#### The Orcale maps x to (yes,) if x is in the convex set K.
#### The Orcale maps x to (no, c) if x is not in the convex set K and cx > cy for all y in K

I misunderstood the numerical stabilization of this orcale and invented the following orcale and algorithm.

# Simple HyperBall Method:

## Distance Orcale for convex set K and precision epsilon:
#### The Orcale maps x to (yes,,) if x is in the convex set K up to precision epsilon
#### The Orcale maps x to (no, c, d) if x is not in the convex set K and cx > cy + d for all y in K
#### d >= epsilon for all inputs. c has euclidean norm 1.

## Method
Given a Distance Orcale for convex set K and precision epsilon

We start with a HyperBall of radius R and center c which contains the convex set K.

We will end with either:
with a point in K, x or
with the guarantee that K does not contain a ball of radius epsilon maybe because K is an empty set.
```
FindPoint(DistanceOrcale,c,R) :
R2 = R *R
do :
  (not_found ,h,d) = DistanceOrcale(c)
  if not_found :
    c - h*d
    R2 = R2 - d
while not_found and R2 > epsilon^2
return (not_found,c)
```
Iteration count: (R/epsilon)^2 
Work per iteration: O(n + orcale_call)
depth per iteration: O(log(n)+ orcale_call)

## Distance Orcale for Linear Program:
We can create a distance orcale for a linear program very simply:
If we normalize all the rows ```ax <= b``` 
Then a violated constraint  ```ax = b+d``` with d >=epsilon.
gives us (no,a,d) otherwise we return yes.

Orcale call work: O(nm) arithmetic operations

Orcale call depth: O(log(nm)) arithmetic operations

# Doggo Method :
We extend the method to a dynamic precision:
## Weak Distance Orcale for convex set K and dynamic precision d and sharpness s
#### The orcale maps x,d to (yes,,) if  x is distance at most sd from K
#### The orcale maps x,d to (no,h,b) if hx > hy + b with b >= d
This has the same start and end behaviour of the simple hyperball method.

## Method
```
FindPoint(WeakDistanceOrcale,c,R)
let s be the sharpness parameter for WeakDistanceOrcale
do:
  (close, halfplane_normal,distance) = WeakDistanceOrcale(c,R/(2s))
  if close :
    R = R/2
  else :
    c = c - halfplane_normal*distance
    R = sqrt(R^2 - distance^2)
while R > epsilon
(found,,) = WeakDistanceOrcale(c,epsilon)
return (found,c)
```
Iteration Count: O(s^2log(R/epsilon) )
work per iteration O(n + orcale call)
depth per iteration O(log(n)+ orcale call)
## Weak Distance Orcale for sparse linear program
Let the program have m constraints over n dimensions with at most s entries per constraint, and at most s constraints which contain a variable.
The distance from the convex polytope represented by a linear program can be bounded by at most Sqrt(sum over all n (constraint_violations^2))*angular_constant
If the constraints violated have right or acute angles, this computation is a tight upper bound.
The angular_constant occur because some constraints have obtuse angle between their normals.
So we can compute that upper bound on the distance from K in O(n + m) work and log(nm) depth.
```
get_halfplane(c) :
Construct a graph G with nodes for each violated constraint, and edges for each shared nonzero entry between two constraints.
Find a random maximal Independent set I on G.
The unnormalized normal vector for the halfplane is the sum of all the violated constraints, and its distance is the sum of all violations divided by the normalization factor of the vector.
```
The number of entries in the unnormalized vector is <=Ls with L verticies in the independent set.
L >= a(G)/s because we can imagine this as s-dimensional matching and those have 1/s maximal approximations.
a(G) >= rows/(1+s^2) because G has degree at most s^2.
Thus magnitude of normalized vector is asymptotically sqrt(rows/s)
and the sum of all violations is roughly still rows/s^3 *Mean_violation.
So the distance is asymptotically sqrt(row)/s^2.5 *Mean_violation
and the distance we are comparing to is sqrt(row) *RMS_violation
So the sharpness is asymptotically s^2.5 * CV(violated constraints)
by the algebra involving rms/mean = Sqrt(mean^2+stdDeviation^2)/mean <= sqrt(2) * (mean/mean+ stdDeviation/mean) = O(CV ).
While the distribution of constraint violations has limited coefficient of variation,
we get a asymptotically constant sharpness orcale.
#### IF CV can be bounded by O(1) P = NC
as there are P-complete sparse linear programs.
#### since the best bound I have on CV  = O(sqrt(m)) this doesn't clearly hold
Work O(m + n)
Depth O(polylog(n)) via results on maximal independent set.[citation needs to be found]
Maybe this would show something on P vs NC if we biased the sampling method for the independent set idk.


# Simplified ellipsoid Methods: The Hyperball methods
# Introduction
So the ellipsoid method is a famous method for solving linear programs and convex programs in general.
The method and its variants have two nice properties:
1. Their runtime is independent of the number of constraints because they access the problem via an orcale.
2. Their runtime is polynomial in the amount of precision needed.
The orcale that the original method uses is a strong separation orcale:

### Separation Orcale for convex set K:
#### The Orcale maps x from R^n to (yes,) if x is in the convex set K.
#### The Orcale maps x from R^n to (no, c) if x is not in the convex set K and cx > cy for all y in K

I misunderstood the numerical stabilization of this orcale and discovered the following orcale and algorithm.

# The Dog Method: A simple HyperBall Method

### Distance Orcale for convex set K and precision epsilon:
#### The Orcale maps x from R^n to (yes,,) if x + error is in the convex set K and the euclidean norm of error is at most epsilon.
#### The Orcale maps x from R^n to (no, c, d) if x is not in the convex set K and cx > cy + d for all y in K
#### where d >= epsilon for all inputs. c has euclidean norm 1.

## Dog Method:
Given a Distance Orcale for convex set K and precision epsilon

We start with a HyperBall of radius R and center c which contains the convex set K.

We will end with either:
with a Ball that intersects K, Ball(center = x,radius = epsilon) or
with the guarantee that K is an empty set.
```
FindBall(DistanceOrcale, center, Radius) :
Let epsilon be the precision of DistanceOrcale
Let n be the dimension of the problem
RadiusSquared = Radius *Radius
do :
  (found, halfplane_normal, distance) = DistanceOrcale(c)
  if not found :
    center = center - halfplane_normal*distance
    RadiusSquared = RadiusSquared - distance^2
while not found and RadiusSquared > epsilon^2
if not found :
  return (false,,)
return (true,c,epsilon)
```
Iteration count: (Radius/epsilon)^2
Work per iteration: O(n + orcale_call)
depth per iteration: O(log(n)+ orcale_call)

### Bad Distance Orcale for Linear Program:
We might attempt to create a distance orcale for a linear program very simply:
If we normalize all the rows ```ax <= b``` 
Then a violated constraint  ```ax = b+d``` with d >=epsilon.
gives us (no,a,d) otherwise we return yes.
Orcale call work: O(nm) arithmetic operations
Orcale call depth: O(log(nm)) arithmetic operations
#### ERROR: This orcale does not guarantee a distance epsilon when it returns yes.
#### One might be able to bound the final distance as less than some problem related constant delta.
## Discussion of Dog Method
This simple method finds a ball in time exponential in precision, but logarithmic depth in dimension, and independent of constraint count.
However, I don't know how to construct a strong distance orcale. What we actually constructed was a weak distance orcale which is defined as follows:
### Weak Distance Orcale for convex set K and precision delta, epsilon
#### The Orcale maps x from R^n to (yes,,) if x + error is in the convex set K and the euclidean norm of error is at most delta.
#### The Orcale maps x from R^n to (no, c, d) if x is not in the convex set K and cx > cy + d for all y in K
#### where d >= epsilon for all inputs. c has euclidean norm 1. Note delta > espilon.
When the Dog Method uses a weak distance orcale, the method needs to return a ball with radius delta upon being found.

# The Doggo Method :
We extend the method to a dynamic precision and implement a better orcale:
### Weak Dynamic Distance Orcale for convex set K and dynamic precision d and sharpness s
#### The orcale maps x,d to (yes,,) if  x is distance at most sd from K
#### The orcale maps x,d to (no,h,b) if hx > hy + b with b >= d
This has the same start and end behaviour of the simple hyperball method.

## Doggo Method Iteration Procedure
```
FindBall(WeakDynamicDistanceOrcale,c,R)
let s be the sharpness parameter for WeakDynamicDistanceOrcale
do:
  (close, halfplane_normal,distance) = WeakDynamicDistanceOrcale(c,R/(2s))
  if close :
    R = R/2
  else :
    c = c - halfplane_normal*distance
    R = sqrt(R^2 - distance^2)
while R > epsilon
(found,,) = WeakDynamicDistanceOrcale(c,epsilon)
return (found,c)
```
Iteration Count: O(s^2log(R/epsilon) )
work per iteration O(n + orcale call)
depth per iteration O(log(n)+ orcale call)

### Weak Dynamic Distance Orcales for sparse linear program
Let the program have m constraints over n dimensions with at most s entries per constraint, and at most s constraints which contain a variable.
```
WeakDynamicDistanceOrcale(c,d):
if d > distance_bound(c) :
  return (yes,,)
(halfplane_normal,distance) = get_halfplane(c)
return (no, halfplane_normal, distance)
```

#### The distance from the convex polytope represented by a linear program can be bounded by:
```
distance_bound(c):
sum = 0
in parallel for constraint (a, b) in linear_program :
  violation = max( ac - b, 0)
  sum += violation^2
return sqrt(sum)*angular_constant
```
If the constraints violated have right or acute angles, this computation is a tight upper bound.
The angular_constant occurs because some constraints have obtuse angle between their normals.
So we can compute that upper bound on the distance from K in O(n + m) work and log(nm) depth.
#### The gap can be found:
```
get_halfplane(c) :
Construct a graph G with nodes for each violated constraint, and edges for each shared nonzero entry between two constraints.
Find a random maximal Independent set I on G.
The unnormalized normal vector for the halfplane is the sum of all the violated constraints, and its distance is the sum of all violations divided by the normalization factor of the vector.
```
This is very much pseudocode, but has a not great shaprness bound.
The number of entries in the unnormalized vector is <=Ls with L verticies in the independent set.
L >= a(G)/s because we can imagine this as s-dimensional matching and those have 1/s maximal approximations.
a(G) >= rows/(1+s^2) because G has degree at most s^2.
Thus magnitude of normalized vector is asymptotically sqrt(rows/s)
and the sum of all violations is roughly still rows/s^3 *Mean_violation. 
(IS THIS TIGHT?)
So the distance is asymptotically sqrt(row)/s^2.5 *Mean_violation
and the distance we are comparing to is sqrt(row) *RMS_violation
So the sharpness is asymptotically s^2.5 * CV(violated constraints)
by the algebra involving rms/mean = Sqrt(mean^2+stdDeviation^2)/mean <= sqrt(2) * (mean/mean+ stdDeviation/mean) = O(CV ).
since the best bound I have on CV  = O(sqrt(m))
So the sharpness is the sparsity ^2.5 * sqrt(number_of_constraints)
Work O(m + n)
Depth O(polylog(n)) via results on maximal independent set.[citation needs to be found]
### Conclusion of Doggo Method
This method for linear programs using this orcale is polynomial in terms of sparsity of linear program, precision, and number of constraints
So it lacks some nice properties of the ellipsoid method
## The Pup Method
This is a potentially universal Paralleization method built ontop of the doggo method.
### Reductions
Start with a circuit we want to parallelize C with G gates of bounded fan-in.
Transform it into a circuit C' with O(G) gates of And,Or,Not, Copy where:
1. And is a fan-in 2 fan out 1 gate that computes logical and.
2. OR is a fan-in 2 fan-out 1 gate that computes logical or.
3. Not is a fan-in 1 fan-out 1 gate that negates the input.
4. Copy is a fan-in 1 fan-out 2 gate that repeates the input.
   
CITATION NEEDED for pre-existing reduction
We can turn this circuit C' into a sparse linear program P
with <=5 occurences per column <=3 occurrences per row.
1. w = AND(u,v)  :  w- u <= 0 ; w-v <= 0 ;  u + v - w <= 1
2. w = OR(u,v)   :  u -w <= 0 ; v- w <= 0;  w -u -v <= 0
3. w = NOT(u)    :  w + u <= 1 ; -w -u <= -1
4. w,v = copy(u) :  u - w <= 0 ; w - v <= 0 ; v - u <= 0
### Modifying the Doggo Method   
We solve this sparse linear program with the same iteration procedure as the Doggo Method, and the same distance bound, but we upgrade and more tightly specify the half plane gap.
```
pup_cut(x) :
Let G be an empty Graph
in parallel for the ith constraint (a,b) in L :
  if ax > b :
    add a vertex i with weight (ax - b)/norm(a) to G
in parallel for the ith dimension of L :
  for each pair (u,v) of nonzero entries in column i of the matrix of L :
    if vertex u and vertex v in G :
      add edge (u,v) to G
IS = random_maximal_independent_set(G)
Let u = 0 vector
in parallel for the member v of IS :
  let (a,b) be vth constraint of L 
  u += weight of v in G * a /norm(a)
return (u/norm(u) , norm(u) )
```
### work bound:
O(ms) work in the first loop,
O(ns^2) work in the second loop,
O(m + ns^2) work in the maximal independent set call (CITATION NEEDS TO BE FOUND)
O(ms) work in the second loop.
O(m) work in the final norm computation.
for our application this is a O(G) work bound.

### depth bound:
coallescing all the verticies should take O(log(m)) depth
Coallescing all the edges should take O(log(m)) depth
The independent set call should take O(poly(log(m))) depth (CITATION NEEDS TO BE FOUND)
The Coallescing of the vectors into a sum should take O(log(m)) depth
The norm computation should also take O(log(n)) depth
So the total depth is O(poly(log(G))) depth

### sharpness bound:
#### Assumption 1: The sum of square of weights of a random maximal independent set is proportional to the sum of square of weights of a random set of equal size. 
#### Assumption 2: The distance of a point x from a polytope P is bounded above by C times the square root of the sum of square constraint violations.
#### Where C depends on the internal angles of P and not on its dimension.
1. maximum_independent_set(G) >= rows / (1+ degree_of_G)
2. degree of G in our case is 5 choose 2 times 3, so 30.
3. maximal_independent_set(G) >= maximum_independent_set(G)/entries_per_row 
4. by imagining this as a entries_per_row dimensional matching problem.
5. thus in our case:
6. maximal_independent_set(G) >= rows/93 
7.since we are summing the vectors in orthogonal directions
8. norm(u) ~ Sqrt(rows/93)*Constraint_violation_RMS
9. So the sharpness is < 10 times the angular_constant times the similarity_constant.
### Bigger Picture
Assuming the sharpness is at most some constant C,
the Pup method for circuits runs in O(PolyLog(G)) depth and O(Glog(G)) work.

Because this claim is a moonshot, I hope to have actual running code before seeking publication.


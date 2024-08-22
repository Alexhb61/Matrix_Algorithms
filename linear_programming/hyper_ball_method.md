# A Simplified ellipsoid Method: the hyperball method
# Introduction
So the ellipsoid method is a famous method for solving linear programs and convex programs in general.
The method and its variants have two nice properties:
1. Their runtime is independent of the number of constraints because they access the problem via an orcale.
2. Their runtime is polynomial in the amount of precision needed.
The orcale that the original method uses is a separation orcale:

#### Separation Orcale for convex set K:
The Orcale maps x to (yes,) if x is in the convex set K.
The Orcale maps x to (no, c) if x is not in the convex set K and cx > cy for all y in K
I misunderstood the numerical stabilization of this orcale and invented the following orcale.

#### Distance Orcale for convex set K and precision epsilon:
The Orcale maps x to (yes,,) if x is in the convex set K up to precision epsilon
The Orcale maps x to (no, c, d) if x is not in the convex set K and cx > cy + d for all y in K
d >= epsilon for all inputs. c has euclidean norm 1.

# Simple HyperBall Method:
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
depth per iteration: O(log(n))

# Distance Orcale for Linear Program:
We can create a distance orcale for a linear program very simply:
If we normalize all the rows ```ax <= b``` 
Then a violated constraint  ```ax = b+d``` with d >=epsilon.
gives us (no,a,d) otherwise we return yes.

Orcale call work: O(nm) arithmetic operations

Orcale call depth: O(log(nm)) arithmetic operations


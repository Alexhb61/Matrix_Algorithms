# Introduction
This paper is concerned with matrix equations of the form Ax=b,
and finding the least squares solution to the problem.
While working on finishing my iterative methods paper,
I had an idea for a simple algorithm to solve systems of linear equations in quadratic time.
Now that I've found a proof of correctness in the full column rank case,
I am typing this up to show to collegues.

# Algorithm
I think this algorithm is simple and neat to use, but its correctness proof is nontrivial.
Here is some psuedocode
```
\\ Assumption A has full column rank (and thus A has no all zero column)
\\ A is a m by n matrix over the reals
\\ b is a m by 1 vector
\\ return the x which is the least square solution
Solve(A,b) 
  Let A = [a_1, A_2 ] \\ where a_1 is a m by 1 vector.
  x_1 = (a_1.b)/(a_1.a_1) \\using the dot product
  if A_2 is not empty :
    x_rest = Solve(A_2, b - a_1*x_1 )
    return [x_1; x_rest] \\ concatentate x_1 to the top of the vector
  else:
    return [x_1] 
```
# Runtime
The algorithm takes O(mn) work and O(nlog(m)) depth.
Proof: Each layer of recursion does 2 dot products on m length vectors,
1 division, then m multiplications, and m subtractions. 
So, the algorithm uses O(m) work O(logm) depth for n iterations;
thus, the claimed amount of total work and total depth holds.
## Parallelism:
This might not overshadow some iterative methods because of its lack of parallelism.
# Correctness:
We will prove that this algorithm finds the least squares solution
by induction on n given the premise that A has full column rank.

### Base Case:
```ax=b ```is least square approximated by ```a^tax=a^tb``` which we solve directly using simple algebra.
### Inductive Case:
Because we have full column rank, we can apply cramer's rule to the equation ```A^tAx=A^tb``` 
and extract a formula for x_1:
```
x_1 = det([a_1,A_2]^t [b A_2])/ det([a_1,A_2]^t[a_1,A_2] )
```
Then we multiply out the matricies:
```
x_1 = det ( [a_1^tb , a_1^tA_2 ; A_2^tb , A_2^tA_2] ) / det ( [a_1^ta_1 , a_1^tA_2 ; A_2^ta_1 , A_2^tA_2] )
```
Then because ```A``` has full column rank, so does ```A_2```; furthermore, ```A_2^tA_2``` is invertible.
Thus we can apply formulas for block matricies and cancel a common ```det(A_2^tA_2)``` from the numerator and denominator.
```
x_1 = (a_1^tb - a_1^t(A_2)(A_2^tA_2)^-1(A_2^t)b)/(a_1^ta_1 - a_1^t(A_2)(A_2^tA_2)^-1(A_2^t)a_1)
```
We can now recognize that ```(A_2)(A_2^tA_2)^-1(A_2^t)``` is an orthogonal projection onto the rowspace of A_2.
Call the projection onto not the rowspace of A_2 P.
```
x_1 = (a_1^tPb)/ (a_1^tPa_1)
```
If we rewrite ```b``` as ```ya_1 + z``` where y is a scalar,
and ```z``` is orthogonal to ```a_1``` .
Then inserting that into the equation we get the formula used in the algorithm with an error term
```
x_1 = (a_1^tP(ya_1+z)/ (a_1^tPa_1)
= y + (a_1^tPz)/(a_1^tPa_1)
= (a_1^tb)/(a_1^ta_1) + (a_1^tPz)/(a_1^tPa_1)
= (a_1^tb)/(a_1^ta_1) +  error
```
The denominator of the error is nonzero because A has full rank and thus ```Pa_1 =/= 0```
We will now argue that the error = 0, by rewriting z = w + v where Pw = 0 and (I-P)v = 0
```
(a_1^tPz) + a_1^t(I-P)z = a_1^tz = 0
a_1^tv + a_1^tw = 0
```
(This feels like the weakest portion of the argument)
Because w is the portion of b that will be solved by the rest of the algorithm (its in the span of the other columns)
we know that ```a_1^tw = 0``` thus ```a_1^tv = 0``` which means ```(a_1^tPz) = 0``` and finally ```error = 0 ```

The recursive call finishes the algorithm because it solves the same equation after moving (a_1*x_1) to the other side, 
and by the inductive hypothesis we have the recursive call's correctness.

# Conclusion

I'd like a second pair of eyes on this, but if correct, this algorithm is big news.

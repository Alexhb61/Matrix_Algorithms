# Introduction
This needs work.
This rough draft claims that Hidden Shift Problem over a product of small primes with a slight failure rate
is NP-hard under randomized polynomial reductions.
# Reduction 1:
0-1 Integer Linear Equality Program with a unique solution is NP-Hard under randomized polynomial reductions.
#### Proof: 
Apply the Valiant–Vazirani theorem to get a Unambiguous SAT instance under randomized reductions.
Turn this into a 3-SAT instance through Karp's well-known reduction.
Use Schaefer's reduction to 1-in-3 SAT which I believe is parsimonious. 
(There are messier parsimonious reductions known) 
We can then write the 1-in-3 SAT as a 0-1 integer linear equality program,
but every clause becomes a row in the matrix that will be set equal to 1.
(If we need to negate variables we can substitue 1-x into the equation's row and adjust b accordingly).

# Reduction 2:
0-1 Integer Linear Equality Program with a unique solution 
can then be reduced to a special case of unique shortest vector problem with following properties:
1. it uses the infinity norm,
2. it has a factor of 2 in size between the shortest and second shortest vector,
3. we know one of the integers which will make up the shortest vector,
4. we have a bound N on the size of integers needed to make the shortest vector.

Given Ax = b as the linear equality program,
The ith main vector of our SVP will use the ith column of A multiplied by 2 
concatenated with ith standard basis vector on dimension n +1.
The last vector of our SVP will use the vector -2*b 
concatenated with an n+1 dimension vector of all -1/2.
#### Claims 1 and 2:
Any solution to Ax=b via 1,0s will produce 0s in the upper half of SVP,
and will produce signed 1/2s in the bottom part.
Any incorrect but still 0-1 solution to Ax =b will reveal a 2 or -2 in the upper half of SVP
Any lattice point in this SVP which is not an a 0-1 but has 1 in the last vector,
the point will have -3/2 or +3/2 or larger in magnitude in the bottom part.
Any lattice point in this SVP which is not all 0s but has a 0 in the last vector,
will have at least magnitude 1.
Any lattice point in this SVP which has -1 in the last vector might have the negation of the previous sections.
Any lattice point in this SVP which has 2 or larger in the last vector will have at least magnitude 1.
Thus a correct solution has infinity norm of 1/2 and the next biggest solution will have infinity norm at least 1.
#### Claim 3 and 4:
The shortest vector will have the last vector as 1, and will only need integers of 1 bit.

# Reduction 3: 

Given an 4-factor-gap SVP with the other listed above properties, we can reduce it to the Hidden Shift Problem.
Given the number N bound, we pick n primes greater than 2(n^2)N, 
and let our n numbers range from  -(P-1)/2 to (P-1)/2 for each prime P.
Then the function f that has the hidden shift will be the rounded version of these n numbers matrix-vector multiplied by all but the known vector,
and the function g that is the shifted version of f will be the rounded version of these n numbers matrix vector multiplied by all 
but the known vector summed with the known vector times its correct quanity.
If Ax rounds by an amount d to the same value as Ay + b then we know that (Ay-Ax+b) has infinity norm at most 2d.
Note that if d is both the magnitude of the rounding and the magnitude of the shortest vector we will not reliably get rounding in the same direction.
Thus we need a gap of ```2+epsilon```, ```4 works```.
Furthermore if ```x+s``` would wrap around, then it fails to show up as hidden shift.
For each number this happens with probability ```(1-1/2n^2) ```
which is a small but nontrivial fail rate which stays small after raising it to the nth power.

#### concern
This barely doesn't work.
The hidden subgroup problem could be built off a 4 gap, and rounding to a known amount would work...

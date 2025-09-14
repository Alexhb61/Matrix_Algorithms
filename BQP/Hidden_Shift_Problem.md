# Introduction:
I'm excited about this.
THIS NEEDS WORK
# Problem:
Given a pair of functions f, g such that f(x) = g(x+s) 
where the plus is performed over some abelian group G. 
Find s. (Hopefully in BQP time)
The functions are accessed via orcales such that they change 0 into f(x) , g(x) in another register.

# Algorithm 1:
## Easy subcase: One bit.
Let s be only the most significant bit (or zero), and G is the cyclic group Z/2^nZ 
Then we can embed this as an abelian hidden subgroup problem on G x G as 
H : ``` x,y ``` mapped to ```LSB(x)*f(y) xor NOT(LSB(x))*g(y) ```
where ```LSB(x)```is the least significant bit of x.
as this is ```H(x,y) = H(x+1, y + s) ```
We can also setup the cosets by:
1. Having 3 n-bit registers starting with zero.
2. Hadamarding the first 2 n-bit registers. 
3. Based on the lowest bit of the first register, apply f or g to the second register into the third.
4. measure the third register.
5. |x0, y> + |x1, y + s > summed over all x is the result, and y is arbitrary.
We can then solve this by doing the appropriate quantum fourier transform and measurements.
1. IQFT(2^n) on both the first and second register.
2. ??? Measure g in the dual group ???

## Bad Reduction
We might be able to reduce the hidden shift 
over the cyclic group Z/2^nZ to the one bit case repeatedly.
We can setup the cosets for the kth bit:
1. Having a 3 n-bit registers starting with zero.
2. Hadamarding the first 2 n-bit registers.
3. Based on the lowest bit of the 1st register, apply f or g to the second register into the third.
4. Measure the third register.
5. Conditional on 1 subtract the known k-1 bits of s.
6. Now Hadamard the top n-k bits of the second register, and measure these bits.
7. Measure the top n-k bits of the 1st register.
8. |z, x0, w, y> + (-1)^r|z, x1, w, y + s> summed over all x, and some arbitrary z and w.
9. r is annoying complicated function of w and x and d.
10. Ignoring the measured parts. This up to a sign error is the input to the single bit solution.
11. |x0, y> + (-1)^r|x1, y + s> summed over all x, and y is arbitary, and r is confusing.
This is where we would apply the QFT but...
## Problem
That sign error seems to break the next step of the one bit algorithm.
At least for my first pass of measuring s on C2 x C2 case.
hmmm...

# Algorithm 2:
## Small number subcase:
### Small number definition:
Let d be a number given in unary as part of the input.
Then we call it small, because we are given it in unary not binary.

Given a hidden shift problem where the abelian group's size is a small number d,
We can solve this in polynomial time with two Quantum fourier transforms, 
and some post-measurement analysis.
### Trial Pseudocode:
Given f(b,x) as an orcale:
1. Trial Start:
2. Setup the balanced superposition ```|b,x>``` for all ```b``` in ```{0,1}```, ```x``` in ```{0...d}```
3. Apply ```f(b,x)``` to a third register and measure it.
4. We now have the state ```|0,x> + |1,x+s> ```
5. Quantum Fourier transform of size```d``` on the second register.
6. we now have the superposition ```Sum_over_y w_d^(x.y)(|0> + w_d^(s.y)|1>)|y>```
7. If we measure ```y```, and record it for each trial.
8. we now have the superposition ```|0> + w_d^(s.y)|1>```
9. Now add enough padding such that the first register can fit a d number,
10. Apply the inverse QFT on this register.
11. The superposition should now be ```sum_over_r :(1+w_d^-r) |s.y + r>```
12. We measure some ```z``` in this register.
13. End trial

So now we have data of the form ```z ~ s.y``` with error centered at zero.
### Post Trial Analysis:
We can analyze this data by brute force picking the ```s ``` from ```{0 ... d-1} ```which
minimizes the error of ```Sum_over_i (z_i - s.y_i)_limit_to_d/2_-d/2 ^2```
We only need to roughly ```O(T*d)``` arithmetic operations where ```T``` is the number of Trials.

### MISSING PART:
How many Trials are needed? I don't currently know.
I would expect ```O(poly(d) )```, but be willing to be wrong.

## Product of small primes case:
Given a hidden shift problem where the abelian group G is a cyclic group of size M
where M is a product of small primes p,
We can roughly reduce this to the small number case by only changing some steps of the trial,
and using the same post measurement analysis we will get the shift mod the small prime which we reduced to.
Then we can use the chinese remainder theorem to get the shift of the full abelian group.

### Trial Modification:
1. Trial Start for prime p:
2. Setup the balanced superposition ```|b,x>``` where ```b``` is in ```{0,1}```, and ```x``` in G.
3. Apply ```f(b,x)``` to a third register and measure it.
4. We now have the state ```|0,x> + |1,x+s> ```
5. Quantum Fourier Transform of size M.
6. We now have the superposition ```Sum_over_y w_M^(x.y)(|0> + w_M^(s.y)|1>)|y>```
7. If we measure ```y```, and we record ```y mod p``` as v,
8. Let the relevant part of the shift be ```s_i*(M/p)^-1 = r mod p```
9. We now have the superposition ```|0> + w_p^(r.v).w_M^n|1>``` where n is a noise value between 0 and M/P-1
10. We now apply a phase shift to the ```|1>``` state of size ```w_p^(-1/2)``` so that the noise is centered.
11. We can now continue from step 9 of the trial for small numbers and use the same post-trial analysis.

### Post Analysis Correction:
We will get the number r from the linear regression rather than s_i but that can easily be computed.
Then we can use the chinese remainder theorem to generate the shift for the whole group.
This will take O(log(M)) different types of trials with O(poly(p)) trials each.

## Other Cases:
Can the other cases be reduced to this product case?

# Conclusion:
Fun ideas, and clear base cases were found.


# Introduction:
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

    

# Inversion is easier than iteration: Bijections which are easy to iterate are easy to invert on a quantum computer.

# Abstract:
This note is a simple exercise in thinking about quantum computing and cybersecurity.

# Introduction:
While thinking about computing, the author read the paper on iterated reversible computation, 
and was surprised to not see any quantum computing results. [citation goes here]
Furthermore, their core result is that there exist reversible bijections computable in P where iterating them is FP^PSPACE hard.
Thus this note is being written to rectify that omission,
and provide more evidence of the claim that inversion is easier than iteration.

# Main idea:
Given an easy-to-iterate bijection F and a target T we want S: F(S) = T, how can a quantum computer find S?

### Key Term:
Function iteration is the repeated composition of the function with itself.
So applying f twice to x is f(f(x)). If we apply f 5 times to x, we are computing f(f(f(f(f(x))))) .

### Key Defintion:
A function F which can be computed in polynomial time poly(|x|)
is easy-to-iterate if we can apply it n times implicitly 
but only doing poly(|x|,|n|) work, rather than poly(|x|, n) work 
where |m| is the bit length of the number or value m. 

If we think of F as a permutation of the n-bit boolean strings,
and think of one disjoint cycle in the permutation which contains T,
then we can imagine any mapping of S to T to be one edge in that cycle.
1. First we can use Shor's order finding algorithm to find the size of that cycle: 
2. starting with 0^2n T as our two registers.
3. Hadmarding the first register to be all possible positions.
Letting y represent one position inside that superposition (and interpretting y as a 2n-bit number).
4. then applying f y times to T (uncomputing any intermediate results).
5. Then measuring the final result of those applications.
6. Then performing the quantum fourier transform on the first register.
7. This after some classical post processing will give us the period r of F's T cycle.
8. Last We apply F r-1 times to T to get S.

# Implications:
For some symmetric key algorithms, people have been curious if they are groups.
(ie multiple applications always correspond to a different key in the keyspace).
This was a serious concern for triple DES. [citation goes here]
If any public-private key algorithm's keyspace corresponds to a group, then
there may be a way to make that algorithm easy to iterate which would then make
the preceeding reduction an attack vector on said algorithm.

Furthermore, this reduction reveals a requirement for quantum-secure one-way-permutations: They must be hard to iterate.

# Conclusion:
This note discusses quantum computing and cryptography and so might have a very small audience, but the author think it was worth typing here.



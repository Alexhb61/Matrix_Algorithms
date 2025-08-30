# Inversion is easier than iteration: Bijections which are easy to iterate are easy to invert on a quantum computer.

# Abstract:
This note is a simple exercise in thinking about quantum computing and cybersecurity.

# Introduction:
While thinking about computing, the author read the paper on iterated reversible computation, 
and was surprised to not see any quantum computing results. [citation goes here]
Thus this note is being written to rectify that omission.

# Main idea:
Given an easy-to-iterate bijection F and a target T we want S: F(S) = T, how can a quantum computer find S?

If we think of F as a permutation of the n-bit boolean strings,
and think of one disjoint cycle in the permutation which contains T,
then we can imagine any mapping of S to T to be one edge in that cycle.
1. First we can use Shor's order finding algorithm to find the size of that cycle: 
2. starting with 0^2n T as our two registers.
3. Hadmarding the first register to be all possible positions y.
4. then applying f y times to T (uncomputing any intermediate results).
5. Then measuring the final result of those applications.
6. Then performing the quantum fourier transform on the first register.
7. This after some classical post processing will give us the period r of F's T cycle.
8. Last We apply F r-1 times to T to get S.

### Key Defintion:
A function F which can be computed in polynomial time poly(|x|)
is easy-to-iterate if we can apply it n times implicitly 
but only doing poly(|x|,|n|) work, rather than poly(|x|, n) work 
where |m| is the bit length of the number or value m.

# Implications:
For some symmetric key algorithms, people have been curious if they are groups.
(ie multiple applications always correspond to a different key in the keyspace).
This was a serious concern for triple DES. [citation goes here]
If any public-private key algorithm's keyspace corresponds to a group, then
there may be a way to make that algorithm easy to iterate which would then make
the preceeding reduction an attack vector on said algorithm.

Furthermore, this reduction reveals a requirement for quantum-secure one-way-permutations: They must be hard to iterate.

# Conclusion:
This note discusses quantum computing and cryptography and so might have a very small audience, but I think it was worth typing here.


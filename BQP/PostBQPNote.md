# PostBQP with reversal: A new charactization of FPSPACE or proof PP = PSPACE ?
# Introduction:
TODO
# Preliminaries:
TODO
# Question:
Is PostBQP closed under reversal of the circuit like BQP? 
As in given we can apply circuit A to state x, can we also apply A^-1 to state y in the same complexity class?
Does this question need more formalization?

# PostBQP with Reversal = FPSPACE:
Are FP^PSPACE and FPSPACE the same class?
## FP^PSPACE hardness:
### Iterating a function in P is hard for FP^PSPACE> [citation goes here]
Let f be the function in question, let n be the iteration we want of f, and let x be the input.
Let C be a circuit which has two registers and maps the state |i, y> to the super position |i,y> -|i-1,f(y)>, and maps |0,y> to |0,y>.
### Claim : C is in PostBQP (with padding) assuming F \in BQP.
1. Start with ```|i, y>```.
2. With a padded 1, hadmard it to get ```|i,y,0> - |i,y,1>``` with normalization constants.
3. If ```i``` == 0, postselect on the third register to get ```|0,y>```
4. else: conditional on the third register, decrement ```i```.
5.       conditional on the third register, apply f to ```y```.
6.       hadmard the third register, and postselect on 0.(this turns it back into a padded zero).
This matrix construction comes from Cook in the 1980s in the definition of DET paper.
### Finishing:
Now if we reverse this circuit and feed the reversal(matrix inverse) ```|n,x>``` it will produce 
the superposition over all i ```|n-i, f^(i)(x) >``` with some normalization constants (that syntax is messy in markdown.)
Then we can post-select on the first register being 0 to get the desired iteration of f on x.
## FPSPACE containment:
PostBQP = PP which is inside FPSPACE.
Reversing a circuit amounts to computing a matrix inverse which for n sized matricies
can be done in log^2(n) space see DET paper for details[citation goes here].
Since these matricies are 2^q size for q qubits,
this amounts to q^2 space.
# Concerns:
This is a rough draft. I might find errors in this before I submit it for publication somewhere.




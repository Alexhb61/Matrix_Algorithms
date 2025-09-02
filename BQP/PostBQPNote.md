# PostBQP, iteration and inverse:  PP = PSPACE 
# Introduction:
TODO
# Preliminaries:
TODO
# Core Lemma: PostBQP closed under reversal 
The Lemma is claiming that when we can apply circuit A to state x, can we also apply A^-1 to state y in the same complexity class.
If we take the definition of PostBQP to be 2 qubit general linear gates (as opposed to unitary gates with postselection),
then clearly we can attempt to reverse a PostBQP circuit by applying the moore-penrose pseudoinverse of the gates in reverse order.
If a principal submatrix of the circuit's matrix is invertible, 
and is a connected component when viewed as an adjacency matrix. 
(ie its rows/columns do not have  nonzero entries outside the submatrix).
Then by the property of block matricies that submatrix will be properly inverted by this approach.
# PostBQP = FP^PSPACE:
## FP^PSPACE hardness:
### Iterating a function in P is hard for FP^PSPACE> [citation goes here]
Let f be the function in question, let n be the iteration we want of f, and let x be the input.
Let C be a circuit which has two registers and maps the state ```|i, y>``` to the super position ```|i,y> -|i-1,f(y)>```, and maps ```|0,y>``` to ```|0,y>```.
### Claim : C is in PostBQP (with padding) assuming f is in BQP.
1. Start with ```|i, y>```.
2. With a padded 1, hadmard it to get ```|i,y,0> - |i,y,1>``` with normalization constants.
3. If ```i``` == 0, postselect on the third register to get ```|0,y>```
4. else:
5.       conditional on the third register, decrement  ```i```.
6.       conditional on the third register, apply f to  ```y```.
7.       hadmard the third register, and postselect on 0.(this turns it back into a padded zero).
(Normalization effects can be ignored by using hadmard matricies with 1's and -1's instead of the unitary ones.)
This matrix construction comes from Cook in the 1980s in the definition of DET paper[citation goes here].
We have applied the matrix which when treating the first register as declaring which block we are in, 
corresponds to a block matrix with Identities along the main diagonal and negative permutation matricies along the second diagonal.
This is the identity matrix minus a nilpotent matrix. 
Thus, by well established properties of nilpotent matricies,
its inverse is the identity matrix plus all natural powers of the nilpotent matrix. 
Noting eventually that all high enough powers are zero.
### Finishing:
Now if we reverse this circuit and feed the reversal(matrix inverse) ```|n,x>``` it will produce 
the superposition over all i ```|n-i, f^(i)(x) >``` (that syntax is messy in markdown.)
Then we can post-select on the first register being 0 to get the desired iteration of f on x.
## FPSPACE containment:
PostBQP = PP which is inside FP^PSPACE [citation goes here]
# Concerns:
This is a rough draft. I might find errors in this before I submit it for publication somewhere.




# The Matrix type
## Motivation
So I kept trying to get a matrix multiplication speed up via some manipulation of triangular matricies, but kept failing.
Then I realized there was a limited subset of matricies the algorithms would work for. Thus it is worth defining those matricies.
## Definition of Super Triangular Matricies
#### Let A_k be an upper triangular matrix with 1's in all of its permitable locations and 0's everwhere needed. Then let B be a k^q dimension square matrix.  B is branching factor k super triangular matrix iff B hadmard_product (A_k raised to the kronecker power q) = B
When the branching factor is 2 and all the nonzero entries are 1, the resulting matrix looks like sierpinski's triangle.
It might also be reasonable to think of these matricies as recursively block triangular.
#### These matricies are closed under addition and multiplication.
If you analyze the k^q dimension super triangular matrix of branching factor k as a k by k block matrix,
Then all of the nonzero blocks in that matrix are k^(q-1) dimension super triangular matricies of branching factor k.
Triangular matricies are closed under addition and multiplication is well known.
Addition and Multiplication of Block Matricies can be seen as the addition and multiplication of the blocks.
Thus, by a simple induction argument the claim holds.
###
# Algorithms
## Quadratic algorithm for multiplication of Super Triangular Matricies of branching factor 2.
Note These matricies have n^(log(3)/log(2)) entries, and so it is not very surprising that they have fast algorithms,
but the algorithm is faster than I would expect.
Let A and B be branching factor 2 super triangular matricies. 
Then we can break A into 4 blocks A_00, A_01, A_10, A_11 (likewise for B) where the indexes refer to row and column.
If we are using the upper triangular definition, then A_10 and B_10 are 0.
Thus ``` C_00 = A_00*B_00 ;  C_10 = 0; C_11 = A_11 * B_11 ; C_01 = A_01 * B_11 + A_00 * B_01 ```
At every level of recursion, we have 4 subproblems of half the size.
At every level of recursion, the additional work can be bounded by n^1.6 because of the limited entries of the result.
Thus the divide and conquer recursion relation is T(n) <= 4 T(n/2) + n^1.6 
Which by the master theorem for divide and conquer algorithms is quadratic work.
Note you'd have to implement a custom matrix storage format to guarantee quadratic work and not get the extra log(n) work factor.
Note Higher branching factors have slower algorithms, but more entries.
## depth 6 qTC circuits for iterated super Triangular Matrix multiplication 
#### depth 6 TC circuits for 2 by 2 triangular matrix mulitplication.
Let ```A_i = [a_i b_i;0 c_i]``` be then n matricies we want to multiply.
Let ```Product(i = 1 to n)(A_i) = [Product(i = 1 to n)(a_i) Sum(j = 1 to n)((Product(i = 1 to j-1)(a_j))b_j(Product(i = j+1 to n)(c_i))); 0 Product(i = 1 to n)(a_i) ]```
This result is similar to the multiplication of 2 by 2 stochastic matricies by (insert citation here).
The depth claim is a result of depth 4 circuits for iterated multiplication, and depth 2 circuit for iterated addition. (CITATION NEEDED)
If we let a_i itself be a triangular matrix, b_i be a vector, and c_i be a scalar, we can then recursively apply this formula to the matrix multiplication part.
And will find that multiplication of n k by k triangular matricies can be done in TC depth 6, n^(k+1) work.
Note that c_i needs to be a scalar in order for the vector and scalar part to distribute over the sum.
#### If A_i is super triangular the so is a_i, b_i and c_i, Thus the problem recursively shows up inside itself with only a sum on the outside.
So an n term product of n by n super triangular matricies of fixed branching factor can be computed in depth 6 TC circuits of quasipolynomial n^O(logn) work.
Note the log(n) term shows up because a k levels of recursion and thus k sums show up when 2^k = n.

I'm considering trying to extend these results...

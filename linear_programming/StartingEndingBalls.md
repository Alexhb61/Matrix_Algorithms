# Introduction:
The ellipsoid method and the hyperball method that is being developed,
both require a starting ball of some volume, and an ending ball of a smaller volume.
The Hyperball method seems to be linear in bit-precision, and is not strongly polynomial.
Note, there are interesting linear programming problems on small (log(n) bit precision) numbers.
# BitPrecision method:
CITATION NEEDED: (video?)
Let phi = bit length of A and b.
Then we can adjust ```Ax<=b``` to be ```Ax<= b+2^(-5*phi) ```
Then a very rough starting ball ``` B(0, sqrt(n)*2^phi) ```
and an ending precision ```2^(-6*phi) ``` .

# Positive Linear Program Bound:
Let ```Ax<=b; x>=0``` be a linear program with only nonnegative entries.
Then we can contain all variables by ``` x_i <= max_j( b_j/a_ij when a_ij != 0 ) ```
in a box by a simple argument.
If that one variable violates a constraint on its own,
then that variable is too large.
## Integer subcase:
Furthermore, If all entries of the input are b-bit integers, then ```x_i <= 2^b```
and we can use a starting ball ```B(0, sqrt(n)*2^b )```
The solution will only be effected by precision up to ```2^(-b-1)/n```
and so we can take that to be our ending precision and our ending radius.

# 0-1 Linear Program Bound:
For enough theoretical cases, the program is all 0,1,-1 numbers and
we know that the solution is inside the unit hypercube ```x_i in [0,1]``` 
In this case, the starting ball can simply be ```B(0, sqrt(n) ) ```
If we only want to know whether there is a nearby integer solution,
then we only need precision up to  ```1/(2n)``` and
thus our ending radius can be this number.


# Introduction
Sometimes I attempt to find fast matrix multiplication algorithms, and this attempt was a near miss.
The angle of approach was to look at ```2^k x 2^k``` matricies as iterated layers of the modifided caley-dickson construction.
This can also be seen as iterated layers of the co-quaternion or split-quaternion. It was in this alternative basis that I NEARLY found a way to multiply 2 by 2 matricies with 5 multiplications.
I can multiply SYMMETRIC 2 by 2 matricies in 5 multiplications. I found 2 ways to do it. If someone has a weird matrix which recursively has this structure, then a fast algorithm exists.

# Preliminary:
The reader needs to be familiar with the multiplication table for coquaternions in order to follow the correctness.
By r in the below formulas, I am referring to the real part of the coquaternion.
Coquaternions can be thought of as 4 dimensional vector space with a distributive multiplication operation and the following identities:
``` jj = kk = 1, ii = -1, ijk = 1 ```

# Formula 1
We are multiplying two 2 by 2 symmetric matricies with 5 multiplies
```
A = [a, b; b, c] and B = [d,e;e,f]
r_A = (a + c) / (2 sqrt(2))
r_B = (d + f) / (2 sqrt(2))
k_A = (a - c) / 2
k_B = (d - f) / 2
j_A = b
j_B = e
m_1 = (r_A + j_A)*(r_B + j_B)
m_2 = (r_A - j_A)*(r_B - j_B)
m_3 = (r_A + k_A)*(r_B + k_B)
m_4 = (r_A - k_A)*(r_B - k_B)
m_5 = (k_A - j_A)*(k_B + j_B)
r_C = ( m_1 + m_2 + m_3 + m_4 ) /2
k_C = sqrt(2) * ( m_3 - m_4 ) /2
j_C = sqrt(2) * ( m_1 - m_2 ) /2
i_C = m_5 -(m_3 + m_4)/2 + (m_1 + m_2)/2
a_C = (r_C + k_C)
d_C = (r_C - k_C)
b_C = (j_C + i_C)
c_C = (j_C - i_C)
Output
C = [a_C b_C; c_C d_C ] 
```
## Correctness
We will show that the middle section matches the multiplication table for coquaternions.
Rule ``` r_C = r_a*r_a + k_a*k_a + j_a*j_a - i_a*i_a ```

Formula ``` r_C = ( m_1 + m_2 + m_3 + m_4 )/2 = ((m_1 + m_2) + (m_3 + m_4))/2 = ((2*r_A*r_B + 2*j_A*j_B) + (2*r_A*r_B  + 2*k_A*k_B))/2 = (4*r_A*r_B + 2*k_A*k_B + 2*j_A*j_B)/2  ``` 
which is correct because of the extra /2 factor showing up in the rr product.

Rule ``` k_C = r_a*k_b + k_a*r_b + i_a*j_b - j_a*i_b ```

Formula ``` k_C = sqrt(2)*( m_3 -m_4 ) =  sqrt(2)*(2*r_A*k_B + 2*k_A*r_B)/2   ``` 
which is correct because each r term has a divide by sqrt 2

Rule ``` j_C = r_a*j_b + j_a*r_b + k_a*i_b - i_a*k_b ```

Formula ``` k_C = sqrt(2)*( m_3 -m_4 ) =  sqrt(2)*(2*r_A*j_B + 2*j_A*r_B)/2  ``` 
which is correct because each r term has a divide by sqrt 2

Rule ``` i_C = r_a*i_b + i_a*r_b + k_a*j_b - j_a*k_b ```

Formula ``` i_C = m5 - (m_3 + m_4)/2 + (m_1 + m_2)/2 = (k_A*k_B - j_A*k_B + k_A*j_B - j_A*j_B ) - (2*r_A*r_B  + 2*k_A*k_B)/2 + (2*r_A*r_B + 2*j_A*j_B)/2 ```
which cancels into the appropriate formula.

# Formula 2
We are multiplying two 2 by 2 symmetric matricies with 5 multiplies
```
A = [a, b; b, c] and B = [d,e;e,f]
r_A = (a + c) / 2
r_B = (d + f) / 2
k_A = (a - c) / 2
k_B = (d - f) / 2
j_A = b
j_B = e
m_1 = (r_A + j_A + k_A) * (r_B + j_A + k_A)
m_2 = (r_A - j_A + k_A) * (r_B - j_B + k_B)
m_3 = (r_A + j_A - k_A) * (r_B + j_B - k_B)
m_4 = (r_A - j_A - k_A) * (r_B - j_B - k_B)
m_5 = j_A * k_B
r_C = (m_1 + m_2 + m_3 + m_4)/4
k_C = (m_1 + m_2 - m_3 - m_4)/4
j_C = (m_1 - m_2 + m_3 - m_4)/4
i_C = (m_1 - m_2 - m_3 + m_4)/4 - 2*m_5
a_C = r_C + k_C
b_C = j_C + i_C
c_C = j_C - i_C
d_C = r_C - k_C
C = [a_C, b_C; c_C d_C]

```
## Correctness
We will show that the middle formula matches the multiplication table for coquaternions.

Part of formula: ```(m_1 + m_2)/2 = (r_A + k_A) * (r_B + k_B) + j_A*j_B```

Part of formula: ```(m_1 - m_2)/2 = (r_A + k_A) * j_B + j_A * (r_B + k_B) ```

Part of formula: ```(m_3 + m_4)/2 = (r_A - k_A) * (r_B -k_B) + j_A*j_B ```

Part of formula: ```(m_3 - m_4)/2 = (r_A - k_A) * j_B + j_A *(r_B - k_B) ```

Rule: ``` r_C = r_a*r_a + k_a*k_a + j_a*j_a - i_a*i_a ```

Formula ``` r_C = (m_1 + m_2 + m_3 + m_4)/4 = ((r_A + k_A) * (r_B + k_B) + j_A*j_B + (r_A - k_A) * (r_B -k_B) + j_A*j_B )/2 = r_A*r_B + k_A*k_B + j_A*j_B ```

Rule ``` k_C = r_a*k_b + k_a*r_b + i_a*j_b - j_a*i_b ```

Formula ``` k_C = (m_1 + m_2 - m_3 - m_4)/4 = ((r_A + k_A) * (r_B + k_B) + j_A*j_B - (r_A - k_A) * (r_B -k_B) - j_A*j_B )/2 = r_A*k_B + k_A*r_B ```

Rule ``` j_C = r_a*j_b + j_a*r_b + k_a*i_b - i_a*k_b ```

Formula ```j_C = (m_1 - m_2 + m_3 - m_4)/4 = ( (r_A + k_A) * j_B + j_A * (r_B + k_B) + (r_A - k_A) * j_B + j_A *(r_B - k_B) )/2 = r_A*j_B +j_A*r_B ```

Rule ``` i_C = r_a*i_b + i_a*r_b + k_a*j_b - j_a*k_b ```

Formula: ```(m_1 - m_2 - m_3 + m_4)/4 - 2*m_5 = ((r_A + k_A) * j_B + j_A * (r_B + k_B) - (r_A - k_A) * j_B - j_A *(r_B - k_B))/2 -2*j_A*k_B = k_A*j_B +j_A*k_B - 2j_A*k_B = k_A*j_B - j_A*k_B  ```

# Recursive Bounds
In order to directly use this formula in an algorithm, the matrix will have 4^k entries, but only 3^k dimensions (degrees of freedom).
That makes the utility far from being all matricies.
## Note:
If you know the output has this same structure, we can lower it to 4 multiplications instead of 5. (in both formulas, the fifth multiplication is only used in the i term).
Moreover, you can square a matrix of this format in roughly n^2 log n work because the 4 multiplications in formula 2 are all squares.

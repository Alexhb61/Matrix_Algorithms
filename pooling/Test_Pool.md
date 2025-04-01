# Introduction
Given a very sensitive and specific test for some condition, how many fewer tests can we use to identify the sick among the healthy?
My idea here is to use techniques from error detecting/correcting codes.
Already in the literature there is a halving procedure, but we might be able to do better.
# Idea 1: Parallel halving:
Here is my early attempt at a better strategy. This is basically just arranging the patients into a hamming code for size N data bits.
## The procedure
Let there be N=2^l patients.
Let the operators believe there is only one or zero patients have the condition we are testing for.
1. Randomly shuffle the patients id's for the procedure.
2. For each bit k of the patient's id, add them to the 2k+ bit(id,k) group. (so if bit 5 is 1, they are added to the 11th group and not the 10th)
3. Test all 2l groups.
4. Interpret participation in at most one negative group as a positive result.
5. Intepret participation in at most L-2 positive groups as a negative result.
## Fail Case:
If 2 patients have the condition instead of 1 or 0, there is roughly a 1 in N chance of a perfect conflict which will render all tests positive.
With more patients having the condition, the probability of a nonsense result goes up.
## Efficiency and critical path:
This is a very short critical path, as all tests can be done in parallel.
This is very efficient as it is 2lg(N) tests, compared to the sqrt(N) test of a matrix based scheme.
## error rates:
```
P(+ procedure | disease) =  P(+ test | disease)^l + l*(1-P(+ test | disease) )*P(+test | disease ) ^ (l-1)
```
For example ``` P(+ test | disease ) = 0.99``` ; l value of 6  lead to ```P(+ procedure | disease ) = .9985 ```
This improves the sensitivity.
```
P( - procedure | no disease && no sick patients) = 1 - (1- P(-test| no disease) )^l - l*(1- P(-test| no disease) )^(l-1)*P(-test|no disease) 
```
For example ```P(- test | no disease ) = 0.99```; l value of 6 leads to ```P(- procedure | no disease && no sick patients) = .999999...```
When Sensistivity and specificity are very high:
```
P( - procedure | no disease && one sick patient) ~~ 1 - l / N
```

# Idea 2: Simple Parallel quartering

## Procedure:
Let there be n = 4^L patients
Let there be less than 4 patients with the condition being tested for.
1. Uniquely and randomly assign patients numbers between 0 and N-1
2. Write each patients number in base 4.
3. For each k from 0 to L-1, Assign each patient to the 4k + b group iff the kth symbol in their number is b.
4. Test all 4L groups in parallel.
5. A patient is positive iff they only participate in positive groups. Otherwise their result is negative.
## Efficiency & critical path:
Both efficiency and critical path are very good 4log(N)/log(4) and 1.
## Fail Case:
If more than 4 patients have the condition, all tests might return positive.
(It might be useful to calculate error rates for 4+ conditioned patients)
## Error rates:
```
P(+ procedure | condtion ) = P (+ test | condition)^L
```
So it is less sensitive.
```
P( - procedure | no condition && <=3 patients are sick)  >= P( - test | no condition && in one unsick group)* P(patient in at least one unsick group)
>= P(-test | no condition) * ( 1 - (3/4)^L) 
```
So it is less specific.
Example:
N = 1024, P(+|yes) = .9998 ; P(-|NO) = .9998

P(+ procedure| yes condition) = .9990

P(- procedure | no condition) >= .7625 worst case for 3 conditioned patients: (243 positive cases)

P(- procedure | no condition) >= .9686 if geometric average number of conditioned groups along each axis is 2. This is also the worst case for 2 conditioned patients. (32 positive cases)

# Generalizing Idea: Parallel Hypercube Search

## Procedure:
0. Let there be <k/2 conditioned patients in N = k^l patient population.
1. Randomly and Uniquely assign patients numbers between 0 and N-1
2. Write each patients number in base k
3. For the ith symbol s in the base k representation of a patient's number, assign them to the  i*k +base_10(s) group.
4. Test all kl groups in parallel.
5. A patient has a positive procedure result if all their groups tests are positive.
## Efficiency and lag time:
kl tests out of k^l efficiency.
Low lag time because all tests can be done in parallel.
## Failure mode:
If more than k conditioned patients are in the population, all tests might return positive which will be a nearly useless result.
## Error Rates:
```
P(+ procedure | condtion ) = P (+ test | condition)^L
```
So it is less specific?

```
P(- procedure | no condition && <k/2 conditioned patients in population) >= P(a patient shares at least one test with no conditioned patients) * P(- test |no condition)
```
So it is less sensitive?
#### Did I mix up sensitive and specific?

## Example
#### N = 1000, k = 10, l = 3
#### Number of tests: 30
#### P(+|yes) = .998 ; P(-|NO) = .998
#### P(+ procedure | conditioned && 5 conditioned patients in population) = .994
#### P(- procedure | no condition && 5 conditioned patients in population) = 7/8*.998 = .873 

# Conclusion
I think higher dimensional thinking can help find better pooling, but I would need to find both the concrete test numbers (specificity and sensitivity) and concrete population numbers ( Cohert whose tests can feasibly be pooled, and condition rates for whatever is being tested). It would also be worthwile for me to familiarize myself more with the metrics of pooling quality.

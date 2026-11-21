# Introduction
The aim with pooling tests for a condition is to minimize the total number of tests performed since they are the main cost of a testing regime.
(as opposed to number of samples collected, or other metrics of work)
When actually implementing a pooling procedure at a facility, 
there is a secondary goal of minimizing turn around time between sample received and diagnosis.
This secondary goal makes the framework of a stream of patients as opposed 
to a whole sample of patients who we interact with all at once more relevant.
# Stream Self-Reduction
For simplified analysis, one might prefer that we treat the stream of incoming patients 
as independent random variables with respect to whether or not they have the condition being tested for.
In order to do pooling in stages where this independence property is preserved,
after pooling k patients in the input queue and receiving a positive result,
those k patients need to be sent to k separate queues, so that patients are not tested with the same pool-mates twice.
The downside to this simplifying self reduction is that it will dramatically increase wait time.
The upside to this simplifying self-reduction is that allows us to write recursive equations
which identify the best sequence of stage sizes WHEN we know the prevalence of disease for a population.
# Dynamic Programming to solve the recursive equations:
Let P be the maximum pool size we are willing to consider.
Let Sensitivity be the probability that a test marks someone positive given the pool contains someone sick with the condition in question.
Let Specificity be the probability that a test marks someone negative given the pool is all healthy relative to the condition in question.
Let Cost be a table indexed by prevalence up to some precision epsilon, and indexed by mean_weight_time up to some precision beta.
(Also keep track of next prevalence and best pool size)
```
All_healthy_Probability(Prevalence,Pool_size) := (1- Prevalence)^Pool_size
Positivity_Rate(Prevalence, Pool_Size) :=  (1 -All_healthy_probability(Prevalence,Pool_size) ).sensitivity + specificity.All_healthy_probability(Prevalence,Pool_size) 
Prevalence_Next(Prevalence, Pool_Size) := IF (Positivity_Rate(Prevalence,Pool_size) > Sensitivity) THEN throw ERROR ELSE sensitivity*Prevalance/Positivity_Rate(Prevalence,Pool_size) 
//NOTE: MWT_current = (Pool_size-1)/2 + Pool_size*MWT_Previous 
Cost(Prevalence, MWT) := IF Prevalence >= Sensitivity THEN 0 
ELIF MWT == 0 THEN 1 ELSE 
let wait = 0, p = 2, best_so_far = 1 
while( wait >= 0 and p < P ) {
let wait = (MWT-(p-1)/2)/p rounded up to nearest integer multiple of beta.
best_so_far = min(best_so_far, 1/p + Cost(Prevalence_Next(Prevalence,p),wait )*Positivity_Rate(Prevalence,p) )
}
```
By noting that prevalence increases under the assumption that prevalence < sensitivity,
 we notice that the subproblem graph is a DAG, and 
we can apply dynamic programming on these equations starting from the prevalence just less than the sensitivity and going down.
This will take time proportional to O(Max_Pool_size*Max_Wait_time/Beta/epsilon)
The units for wait time are patients.
Further optimizations might be feasible.
# Caveats: Other_Costs, FNR, sample limitations
Since this is a meta-protocol, I'd have to actually run it and read the results for the relevant prevalence and wait time
before estimating other costs, and FNR.
This meta-protocol also requires knowing the sensitivity and specificity of the test it is going to be built around,
but we could brute force the possible sensitivities and specificities up to some limited precision.
We know that the final false positive rate will be greater than the sensitivity, but we don't know the false negative rate.
For sample stability, we might need to set a maximum wait time or maximum number of stages,
and this scheme doesn't address those limitations.
# Conclusion
Under the core assumption of streaming model with independent random variables of patients,
this scheme generates the pareto frontier of the tradeoff between mean wait time and cost per patient 
at a given disease prevalence with a given test specificity and sensitivity. 
Future work will look into:
Does this result in notable savings for any major disease?
Can these protocols be simplified at all?
How to address the sample limitations of this scheme?


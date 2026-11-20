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
Let Cost be a table indexed by prevalence up to some precision epsilon.
(Also keep track of next prevalence and best pool size)

Positivity_Rate(Prevalence, Pool_Size) :=  1 - (1- Prevalence)^Pool_size

Prevalence_Next(Prevalence, Pool_Size) := Prevalance/Positivity_Rate(Prevalence,Pool_size) //P(+|sick) is approximately 1.

Cost(Prevalence) := min(1, min_{p from 2 to P} ( 1/p + Cost(Prevalence_Next(Prevalence,p) )*Positivity_Rate(Prevalence,p) ) )

By noting that the prevalence is at most 1, we notice that the subproblem graph is a DAG, and 
we can apply dynamic programming on these equations starting from the prevalence closest to 1 and going down.
This will take time proportional to O(Max_Pool_Size/epsilon)
Further optimizations might be feasible.
# Caveats: Costs, wait times, Sensitivity and Specificity
Since this is a meta-protocol, I'd have to actually run it and reading the results for the relevant prevalence
before estimating costs, wait times, and sensitivity. 
For specificity, we don't diagnosis a patient as having the disease until we reach 1 patient test, so it is preserved.

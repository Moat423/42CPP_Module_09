# Ford-Johnsen Algorithm or the Merge-Insertion Algorithm

also: the algorithm that approaches the least amount of comparisons theoretically needed to sort a list of n elements.
For under eleven Elements, it has the best worse case performance for the amount of comparisons needed to sort a list of n elements.
For this ford johnson algorithm, i would like to create a lookup table in every recursion table.

For n≤11, Ford-Johnson achieves the theoretical minimum number of comparisons.
Once n>11, the algorithm starts to exceed that lower bound, but still remains one of the most efficient sorting algorithms regarding comparisons.

[lower bounds for sort 16, 17, 18 numbers](https://arxiv.org/pdf/2206.05597): 
16:46
17:50
18:54


### ammount of comparisons of the Ford-Johnsen Algorithm

approximation or heuristic:
\[
F(n) = \sum_{k=1}^{n} \left\lceil \log_2 \left(\frac{3}{4}k \right) \right\rceil
\]

[actual average](https://elib.uni-stuttgart.de/server/api/core/bitstreams/34a2eb5d-82a2-463f-8c3d-234ab8076a4c/content)
\[
\boxed{
F(n) = F\left(\left\lfloor \frac{n}{2} \right\rfloor\right) + F\left(\left\lceil \frac{n}{2} \right\rceil\right) + S(n)
}
\]
\[
S(n) = \text{worst-case insertion comparison count, optimized using Jacobsthal numbers and binary search}
also:
𝑆 ( 𝑛 ) = ⌊ 𝑛 2 ⌋ + 𝐺 ( ⌈ 𝑛 2 ⌉ ) S(n)=⌊ 2 n ⌋+G(⌈ 2 n ⌉)
\]

from this [book](https://seriouscomputerist.atariverse.com/media/pdf/book/Art%20of%20Computer%20Programming%20-%20Volume%203%20(Sorting%20&%20Searching).pdf) we can get a table with the minimum comparison count (page 195)


| n |   1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 |
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
|⌈ln  n!⌉  | 0 | 1 | 3 | 5 | 7 | 10 | 13 | 16 | 19 | 22 | 26 | 29 | 33 | 37 | 41 | 45 | 49|
|B(n)  | 0 | 1 | 3 | 5 | 8 | 11 | 14 | 17 | 21 | 25 | 29 | 33 | 37 | 41 | 45 | 49 | 54|
|L(n) | 0 | 1 | 3 | 5 | 9 | 11 | 14 | 17 | 25 | 27 | 30 | 33 | 38 | 41 | 45 | 49 | 65|


the first line is the one with the theoretical minimum.

## my naive approach

I will pair, create a table, assign an index to each pair and then separate the large elements from that table and attribute them with the index, so that when i give them into the next deeper level of pairing as an input, the lookup table created at that point will contain elements paired with an index. These are paired with the new index and from that table i extract the large elements and attribute them with the new index from the current table. That way, at each depth level, there is a table, from which i will then be able to build the main chain back up again, containing information from the previous depth level, so that i can look up the correct elements there. That means, once the pairing is all done, it will return and merge. The merging will use the index table at that step, to create a main chain that is ready for insertion from the vector that was returned in pairing. It will look up the larger element at the index in the order of the returned vec from pairing and then copy that element into a new main chain. This element will contain information for it's value, as well as it's index for the previous recursion level, which is not yet relevant. But the insertion will take place with elements from the table. Then this function will return and return it's modified vector to the previous depth level and so on.

#### generateing the insertion sequence:

pseudocode:

take jabobssequence numbers
	0 1 1 3 5 11 ... with n numbers
jacobs decreased by one, starting from index 2:
	0 2 4 10 ...
new sequence: 0 2  1 4  3 10 9 8 7 6 5... 
starting from index 3 of jabobssequence, decrease number by one to get index
and add inbetween missing numbers while (number - 1 > (current index in modified jacobs sequence - 1))
(for n - 1)


I am thinking, that using a vector and this convoluted lookup table is just making it much more complex right. now. let's go with a list instead.
the list will have its value and a pointer to its smaller pair. The overload for the comparison operators you don't have to write. I can do that.
There will be a largeElement list that i won't modify unlike the main
if we have a pointer, then in the insertion stage, we instantly know the lower_bound.

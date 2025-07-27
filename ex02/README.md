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

I was thinking we could use previousIndex in combination with originalIndex. and before we recourse, we just set originalIndex to the new pair index and the previous index to the original index before its rewrite. and then when we go back up, we do the same thing backwards, that way, we can have an actual lookup. so when we are at the deepest level, we move previoursIndex to originalIndex. and with original index, we then know which elements from largerElements we need to take to build the current mainChain. these would have the correct index relationship for this level. but we actually already need to prepare every element for the next step, by moving previous index to original index. OR actually, even better, after the recursive call, we just work with previousIndex for the mergeInsertion. and while we do that, we use the elements from before the function fordJohnsonSort returned to give us the correct order, and build the main chain from elements from the beginning. and the pend chain as well. then on the next level we can use that prev info.

so lets suppose a little like this:

8 1 4 9 2 7 3 6
pair and sort
|idx| smaller| larger|
| 0 | 1 | 8 |
| 1 | 4 | 9 |
| 2 | 2 | 7 |
| 3 | 3 | 6 |

recurse with:
(8,0,-1)(9,1,-1)(7,2,-1)(6,3,-1)

pair and sort:
|idx| smaller| larger|
| 0 | 8 | 9 |
| 1 | 6 | 7 |

recurse with:
(9,0,1)(7,1,2)

pair and sort:
| 0 | 7 | 9 |
rec with
(9,0,0)
-->return 
look up idx 0 (previousIndex)
take (9,0,1) into mainChain, insert (7,1,2)
return (7,1,2)(9,0,1)
look up idx 2 and 1 once for mainChain, once for pendChain
....
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

#### example sorting

from code with these print statements to see what is going on:
```C++
std::vector<PmergeMe::ElementInfo>	PmergeMe::mergeInsertElements(
		const std::vector<ElementInfo>& lookupSortedSequence, const std::vector<ElementInfo>& pendChain, std::vector<size_t> jacobsthalNumbers)
{
	std::vector<ElementInfo> mainChain = lookupSortedSequence;
	mainChain.insert(mainChain.begin(), pendChain[0]);
	if (pendChain.size() == 1)
		return (mainChain);
	size_t previousJacob = 0;
	std::cout << "mainChain after inserting first element: " << std::endl;
	printContainer(mainChain);
	for (size_t jacobsIndex = 2; jacobsIndex < jacobsthalNumbers.size(); jacobsIndex++)
	{
		size_t currentJacob = (jacobsthalNumbers[jacobsIndex] - 1 ) < pendChain.size() - 1
			? (jacobsthalNumbers[jacobsIndex] - 1) : pendChain.size() - 1;
		std::cout << "currentJacob: " << currentJacob << std::endl;
		std::cout << "previousJacob: " << previousJacob << std::endl;
		for (size_t pendChainIndexToInsert = currentJacob; pendChainIndexToInsert > previousJacob; pendChainIndexToInsert--)
		{
			std::cout << "Inserting element at pendChainIndexToInsert: " << pendChainIndexToInsert << std::endl;
			const ElementInfo& elemToInsert = pendChain[pendChainIndexToInsert];
			size_t bound = mainChain.size();
			if (pendChainIndexToInsert < lookupSortedSequence.size())
			{
				std::cout << "Looking for bound for element: " << elemToInsert << std::endl;
				std::cout << "Partner Element from mainChain: " << lookupSortedSequence[pendChainIndexToInsert] << std::endl;
				for (size_t k = 0; k < mainChain.size(); k++)
				{
					if (lookupSortedSequence[pendChainIndexToInsert].value == mainChain[k].value)
					{
						bound = k;
						break;
					}
				}
				std::cout << "Element at upperBound in mainChain: " << mainChain[bound] << std::endl;
			}
			else
			{
				std::cout << "pendChainIndexToInsert is out of bounds for lookupSortedSequence because it is STRAGGLER, using mainChain size as bound" << std::endl;
			}
			//binary search within bounded range
			std::cout << "Inserting element: " << elemToInsert << " with lower_bound at index: " << bound << std::endl;
			std::vector<ElementInfo>::iterator insertionPoint = std::lower_bound(
					mainChain.begin(), mainChain.begin() + bound,
					elemToInsert);
			mainChain.insert(insertionPoint, elemToInsert);
			std::cout << "mainChain after insertion: " << std::endl;
			printContainer(mainChain);
			std::cout << "--------------------"  << std::endl;
		}
		previousJacob = currentJacob;
	}
	return (mainChain);
}


std::vector<PmergeMe::ElementInfo>	PmergeMe::fordJohnsonSort( std::vector<ElementInfo> &vec)
{
	std::cout << CYN "Level: " << level++ << RESET << std::endl;
	std::cout << "fordJohnsonSort called with: "  << std::endl;
	printContainer(vec);
	std::vector<ElementInfo> elements = vec;
	if (vec.size() <= 1)
	{
		elements[0].previousIndex = elements[0].originalIndex;
		elements[0].originalIndex = 0;
		std::cout << RED << "Base case reached with single element: " <<RESET<< elements[0] << std::endl;
		return (elements);
	}
	// pair and extract ( larger is in odd position)
	std::vector<ElementInfo> largerElements;
	size_t	elementsIndex = 0;
	while (elementsIndex < elements.size() && elementsIndex + 1 < elements.size())
	{
		if (elements[elementsIndex] > elements[elementsIndex + 1])
			std::swap(elements[elementsIndex], elements[elementsIndex + 1]);
		elements[elementsIndex].previousIndex = elements[elementsIndex].originalIndex;
		elements[elementsIndex].originalIndex = elementsIndex;
		elements[elementsIndex + 1].previousIndex = elements[elementsIndex + 1].originalIndex;
		elements[elementsIndex + 1].originalIndex = elementsIndex + 1;
		largerElements.push_back(elements[elementsIndex + 1]);
		elementsIndex += 2;
	}
	ElementInfo	straggler;
	bool		hasStraggler = (elements.size() % 2 == 1);
	if (hasStraggler)
	{
		if (elementsIndex  + 1 != elements.size())
			throw std::logic_error("elementsIndex when detected a straggler is not the last element");
		// set with marker sizemax to mark straggler inside chain
		straggler.value = elements.back().value;
		straggler.originalIndex = std::numeric_limits<size_t>::max();
		straggler.previousIndex = elements.back().originalIndex;
		elements.pop_back();
		std::cout << BLUE << "hasStraggler is true, straggler: " << straggler << RESET << std::endl;
	}
	if (elements.size() / 2 != largerElements.size())
		throw std::logic_error("Size mismatch");
	std::cout << "largerElements: "  << std::endl;
	printContainer(largerElements);
	std::vector<ElementInfo> sortedLarger = fordJohnsonSort(largerElements);

	std::cout << CYN "returned to Level: " << --level << RESET << std::endl;
	std::cout << "with sortedLarger: "  << std::endl;
	printContainer(sortedLarger);
	std::vector<ElementInfo> pendChain;
	pendChain.reserve(sortedLarger.size() + (1 & hasStraggler));
	for (size_t i = 0; i < sortedLarger.size(); i++)
	{
		sortedLarger[i] = (elements[sortedLarger[i].previousIndex]);
		pendChain.push_back(elements[sortedLarger[i].originalIndex - 1]);
	}
	std::cout << "sortedLarger after reverting indices to use as mainChain: "  << std::endl;
	printContainer(sortedLarger);
	std::cout << "pendChain: "  << std::endl;
	printContainer(pendChain);
	if (pendChain.size() == 0)
		throw std::logic_error("pendChain is empty, but should not be");
	std::vector<size_t> jacobsthalNumbers = generateJacobsthalNumbers(pendChain.size() + 2);
	std::cout << "jacobsthalNumbers: "  << std::endl;
	printContainer(jacobsthalNumbers);
	std::cout << "size of pendChain: " << pendChain.size() << std::endl;
	if (hasStraggler && pendChain.size() == jacobsthalNumbers.back() - 1)
	{
		std::cout << "pendChain size + 1 is equal to last jacobsthal number, inserting straggler into pendChain: " << straggler << std::endl;
		hasStraggler = false;
		pendChain.push_back(straggler);
	}
	std::cout << "pendChain after possibly inserting straggler: "  << std::endl;
	printContainer(pendChain);
	std::vector<ElementInfo> mainChain = mergeInsertElements(
			sortedLarger, pendChain, jacobsthalNumbers);
	if (hasStraggler)
	{
		std::cout << "hasStraggler is true, inserting straggler into mainChain" << straggler << std::endl;
		std::vector<ElementInfo>::iterator insertionPoint = std::lower_bound(
				mainChain.begin(), mainChain.end(),
				straggler);
		mainChain.insert(insertionPoint, straggler);
	}
	std::cout << "after insertion, before returning: "  << std::endl;
	printContainer(mainChain);
	return mainChain;
}
```

Before: 5 7 3 9 4 10 2 1 6 8
Level: 1
fordJohnsonSort called with:
[val: 5, origIdx: 0, prevIdx: 18446744073709551615] [val: 7, origIdx: 1, prevIdx: 18446744073709551615] [val: 3, origIdx: 2, prevIdx: 18446744073709551615] [val: 9, origIdx: 3, prevIdx: 18446744073709551615] [val: 4, origIdx: 4, prevIdx: 18446744073709551615] [val: 10, origIdx: 5, prevIdx: 18446744073709551615] [val: 2, origIdx: 6, prevIdx: 18446744073709551615] [val: 1, origIdx: 7, prevIdx: 18446744073709551615] [val: 6, origIdx: 8, prevIdx: 18446744073709551615] [val: 8, origIdx: 9, prevIdx: 18446744073709551615]
largerElements:
[val: 7, origIdx: 1, prevIdx: 1] [val: 9, origIdx: 3, prevIdx: 3] [val: 10, origIdx: 5, prevIdx: 5] [val: 2, origIdx: 7, prevIdx: 6] [val: 8, origIdx: 9, prevIdx: 9]
Level: 2
fordJohnsonSort called with:
[val: 7, origIdx: 1, prevIdx: 1] [val: 9, origIdx: 3, prevIdx: 3] [val: 10, origIdx: 5, prevIdx: 5] [val: 2, origIdx: 7, prevIdx: 6] [val: 8, origIdx: 9, prevIdx: 9]
hasStraggler is true, straggler: [val: 8, origIdx: 18446744073709551615, prevIdx: 9]
largerElements:
[val: 9, origIdx: 1, prevIdx: 3] [val: 10, origIdx: 3, prevIdx: 5]
Level: 3
fordJohnsonSort called with:
[val: 9, origIdx: 1, prevIdx: 3] [val: 10, origIdx: 3, prevIdx: 5]
largerElements:
[val: 10, origIdx: 1, prevIdx: 3]
Level: 4
fordJohnsonSort called with:
[val: 10, origIdx: 1, prevIdx: 3]
Base case reached with single element: [val: 10, origIdx: 0, prevIdx: 1]
returned to Level: 4
with sortedLarger:
[val: 10, origIdx: 0, prevIdx: 1]
sortedLarger after reverting indices to use as mainChain:
[val: 10, origIdx: 1, prevIdx: 3]
pendChain:
[val: 9, origIdx: 0, prevIdx: 1]
jacobsthalNumbers:
1 1 3
size of pendChain: 1
pendChain after possibly inserting straggler:
[val: 9, origIdx: 0, prevIdx: 1]
after insertion, before returning:
[val: 9, origIdx: 0, prevIdx: 1] [val: 10, origIdx: 1, prevIdx: 3]
returned to Level: 3
with sortedLarger:
[val: 9, origIdx: 0, prevIdx: 1] [val: 10, origIdx: 1, prevIdx: 3]
sortedLarger after reverting indices to use as mainChain:
[val: 9, origIdx: 1, prevIdx: 3] [val: 10, origIdx: 3, prevIdx: 5]
pendChain:
[val: 7, origIdx: 0, prevIdx: 1] [val: 2, origIdx: 2, prevIdx: 7]
jacobsthalNumbers:
1 1 3
size of pendChain: 2
pendChain size + 1 is equal to last jacobsthal number, inserting straggler into pendChain: [val: 8, origIdx: 18446744073709551615, prevIdx: 9]
pendChain after possibly inserting straggler:
[val: 7, origIdx: 0, prevIdx: 1] [val: 2, origIdx: 2, prevIdx: 7] [val: 8, origIdx: 18446744073709551615, prevIdx: 9]
mainChain after inserting first element:
[val: 7, origIdx: 0, prevIdx: 1] [val: 9, origIdx: 1, prevIdx: 3] [val: 10, origIdx: 3, prevIdx: 5]
currentJacob: 2
previousJacob: 0
Inserting element at pendChainIndexToInsert: 2
pendChainIndexToInsert is out of bounds for lookupSortedSequence because it is STRAGGLER, using mainChain size as bound
Inserting element: [val: 8, origIdx: 18446744073709551615, prevIdx: 9] with lower_bound at index: 3
mainChain after insertion:
[val: 7, origIdx: 0, prevIdx: 1] [val: 8, origIdx: 18446744073709551615, prevIdx: 9] [val: 9, origIdx: 1, prevIdx: 3] [val: 10, origIdx: 3, prevIdx: 5]
--------------------
Inserting element at pendChainIndexToInsert: 1
Looking for bound for element: [val: 2, origIdx: 2, prevIdx: 7]
Partner Element from mainChain: [val: 10, origIdx: 3, prevIdx: 5]
Element at upperBound in mainChain: [val: 10, origIdx: 3, prevIdx: 5]
Inserting element: [val: 2, origIdx: 2, prevIdx: 7] with lower_bound at index: 3
mainChain after insertion:
[val: 2, origIdx: 2, prevIdx: 7] [val: 7, origIdx: 0, prevIdx: 1] [val: 8, origIdx: 18446744073709551615, prevIdx: 9] [val: 9, origIdx: 1, prevIdx: 3] [val: 10, origIdx: 3, prevIdx: 5]
--------------------
after insertion, before returning:
[val: 2, origIdx: 2, prevIdx: 7] [val: 7, origIdx: 0, prevIdx: 1] [val: 8, origIdx: 18446744073709551615, prevIdx: 9] [val: 9, origIdx: 1, prevIdx: 3] [val: 10, origIdx: 3, prevIdx: 5]
returned to Level: 2
with sortedLarger:
[val: 2, origIdx: 2, prevIdx: 7] [val: 7, origIdx: 0, prevIdx: 1] [val: 8, origIdx: 18446744073709551615, prevIdx: 9] [val: 9, origIdx: 1, prevIdx: 3] [val: 10, origIdx: 3, prevIdx: 5]
sortedLarger after reverting indices to use as mainChain:
[val: 2, origIdx: 7, prevIdx: 6] [val: 7, origIdx: 1, prevIdx: 1] [val: 8, origIdx: 9, prevIdx: 9] [val: 9, origIdx: 3, prevIdx: 3] [val: 10, origIdx: 5, prevIdx: 5]
pendChain:
[val: 1, origIdx: 6, prevIdx: 7] [val: 5, origIdx: 0, prevIdx: 0] [val: 6, origIdx: 8, prevIdx: 8] [val: 3, origIdx: 2, prevIdx: 2] [val: 4, origIdx: 4, prevIdx: 4]
jacobsthalNumbers:
1 1 3 5
size of pendChain: 5
pendChain after possibly inserting straggler:
[val: 1, origIdx: 6, prevIdx: 7] [val: 5, origIdx: 0, prevIdx: 0] [val: 6, origIdx: 8, prevIdx: 8] [val: 3, origIdx: 2, prevIdx: 2] [val: 4, origIdx: 4, prevIdx: 4]
mainChain after inserting first element:
[val: 1, origIdx: 6, prevIdx: 7] [val: 2, origIdx: 7, prevIdx: 6] [val: 7, origIdx: 1, prevIdx: 1] [val: 8, origIdx: 9, prevIdx: 9] [val: 9, origIdx: 3, prevIdx: 3] [val: 10, origIdx: 5, prevIdx: 5]
currentJacob: 2
previousJacob: 0
Inserting element at pendChainIndexToInsert: 2
Looking for bound for element: [val: 6, origIdx: 8, prevIdx: 8]
Partner Element from mainChain: [val: 8, origIdx: 9, prevIdx: 9]
Element at upperBound in mainChain: [val: 8, origIdx: 9, prevIdx: 9]
Inserting element: [val: 6, origIdx: 8, prevIdx: 8] with lower_bound at index: 3
mainChain after insertion:
[val: 1, origIdx: 6, prevIdx: 7] [val: 2, origIdx: 7, prevIdx: 6] [val: 6, origIdx: 8, prevIdx: 8] [val: 7, origIdx: 1, prevIdx: 1] [val: 8, origIdx: 9, prevIdx: 9] [val: 9, origIdx: 3, prevIdx: 3] [val: 10, origIdx: 5, prevIdx: 5]
--------------------
Inserting element at pendChainIndexToInsert: 1
Looking for bound for element: [val: 5, origIdx: 0, prevIdx: 0]
Partner Element from mainChain: [val: 7, origIdx: 1, prevIdx: 1]
Element at upperBound in mainChain: [val: 7, origIdx: 1, prevIdx: 1]
Inserting element: [val: 5, origIdx: 0, prevIdx: 0] with lower_bound at index: 3
mainChain after insertion:
[val: 1, origIdx: 6, prevIdx: 7] [val: 2, origIdx: 7, prevIdx: 6] [val: 5, origIdx: 0, prevIdx: 0] [val: 6, origIdx: 8, prevIdx: 8] [val: 7, origIdx: 1, prevIdx: 1] [val: 8, origIdx: 9, prevIdx: 9] [val: 9, origIdx: 3, prevIdx: 3] [val: 10, origIdx: 5, prevIdx: 5]
--------------------
currentJacob: 4
previousJacob: 2
Inserting element at pendChainIndexToInsert: 4
Looking for bound for element: [val: 4, origIdx: 4, prevIdx: 4]
Partner Element from mainChain: [val: 10, origIdx: 5, prevIdx: 5]
Element at upperBound in mainChain: [val: 10, origIdx: 5, prevIdx: 5]
Inserting element: [val: 4, origIdx: 4, prevIdx: 4] with lower_bound at index: 7
mainChain after insertion:
[val: 1, origIdx: 6, prevIdx: 7] [val: 2, origIdx: 7, prevIdx: 6] [val: 4, origIdx: 4, prevIdx: 4] [val: 5, origIdx: 0, prevIdx: 0] [val: 6, origIdx: 8, prevIdx: 8] [val: 7, origIdx: 1, prevIdx: 1] [val: 8, origIdx: 9, prevIdx: 9] [val: 9, origIdx: 3, prevIdx: 3] [val: 10, origIdx: 5, prevIdx: 5]
--------------------
Inserting element at pendChainIndexToInsert: 3
Looking for bound for element: [val: 3, origIdx: 2, prevIdx: 2]
Partner Element from mainChain: [val: 9, origIdx: 3, prevIdx: 3]
Element at upperBound in mainChain: [val: 9, origIdx: 3, prevIdx: 3]
Inserting element: [val: 3, origIdx: 2, prevIdx: 2] with lower_bound at index: 7
mainChain after insertion:
[val: 1, origIdx: 6, prevIdx: 7] [val: 2, origIdx: 7, prevIdx: 6] [val: 3, origIdx: 2, prevIdx: 2] [val: 4, origIdx: 4, prevIdx: 4] [val: 5, origIdx: 0, prevIdx: 0] [val: 6, origIdx: 8, prevIdx: 8] [val: 7, origIdx: 1, prevIdx: 1] [val: 8, origIdx: 9, prevIdx: 9] [val: 9, origIdx: 3, prevIdx: 3] [val: 10, origIdx: 5, prevIdx: 5]
--------------------
after insertion, before returning:
[val: 1, origIdx: 6, prevIdx: 7] [val: 2, origIdx: 7, prevIdx: 6] [val: 3, origIdx: 2, prevIdx: 2] [val: 4, origIdx: 4, prevIdx: 4] [val: 5, origIdx: 0, prevIdx: 0] [val: 6, origIdx: 8, prevIdx: 8] [val: 7, origIdx: 1, prevIdx: 1] [val: 8, origIdx: 9, prevIdx: 9] [val: 9, origIdx: 3, prevIdx: 3] [val: 10, origIdx: 5, prevIdx: 5]
Time taken: 1.49600 ms
After:  1 2 3 4 5 6 7 8 9 10
size is same? yes
is sorted: yes
comparison count: 22

#include "PmergeMe.hpp"
#include "Debug.hpp"
#include <algorithm>
#include <exception>
#include <iterator>
#include <list>
#include <stdexcept>
#include <vector>

size_t PmergeMe::comparisonCount = 0;

// Default Constructor
PmergeMe::PmergeMe( void )
{
	debug("Default Constructor called");
}

// Parameterized Constructor
// PmergeMe::PmergeMe( std::string value )
// {
// 	debug("Parameterized Constructor called");
// }

// Destructor
PmergeMe::~PmergeMe()
{
	debug("Destructor called");
}

// Copy Constructor
PmergeMe::PmergeMe(const PmergeMe &copy)
{
	*this = copy;
}

// Copy Assignment Operator
PmergeMe& PmergeMe::operator=( const PmergeMe &assign )
{
	if (this != &assign)
	{
		//nothing
	}
	return (*this);
}

//changes vec to that every (n*2) is the bigger number of a pair of (n*2-1,n*2)
// void	PmergeMe::sortInPair( std::vector<ElementInfo> &vec )
// {
// 	for (size_t i = 0; i < vec.size() - 1; i += 2)
// 		if (vec[i] > vec [i + 1])
// 			std::swap(vec[i], vec[i + 1]);
// }

// takes either every (i*2 -n)th elemement
// std::vector<PmergeMe::ElementInfo>	PmergeMe::vectorFromEverySecondElement( const std::vector<ElementInfo> vec , int n)
// {
// 	if (n != 0 && n != 1)
// 		return (std::vector<ElementInfo>());
// 	std::vector<ElementInfo>	Elements;
// 	Elements.reserve(vec.size() / 2);
// 	for (size_t i = n; i < vec.size(); i += 2)
// 		Elements.push_back(vec[i]);
// 	return (Elements);
// }

// build chain from elements, to
// make a duplicate of largeElements or smallElements, but with previousIndex set to -1 and original
// index to what was prevIndex in largeElements
// std::vector<PmergeMe::ElementInfo>	PmergeMe::buildChain( const std::vector<ElementInfo> &elementsList )
// {
// 	std::vector<ElementInfo>	chain;
// 	chain.reserve(elementsList.size());
// 	for (size_t i = 0; i < elementsList.size(); i++)
// 	{
// 		ElementInfo e = { elementsList[i].value, elementsList[i].previousIndex, -1 };
// 		chain.push_back(e);
// 	}
// 	return (chain);
// }

// size_t getGroupSize(size_t n) {
//     if (n <= 2) return 2;
//     return (1U << (n-2)) + getGroupSize(n-2);
// }

// void	PmergeMe::insertElements(std::vector<ElementInfo>& mainChain, const std::vector<ElementInfo>& pendChain, const std::vector<int> jacobsthalNumbers)
// {
// 	if (pendChain.empty()) return;
// 	mainChain.insert(mainChain.begin(), pendChain[0]);
// 	if (pendChain.size() == 1)
// 		return ;
// }

std::vector<size_t>	PmergeMe::generateJacobsthalNumbers(size_t n)
{
	std::vector<size_t>	sequence;
	sequence.reserve(n);
	sequence.push_back(1);
	if (n == 1)
		return (sequence);
	sequence.push_back(1);
	size_t	a = 1;
	size_t	b = 1;
	for (size_t i = 2; i < n; i++)
	{
		size_t next = b + 2 * a;
		if (next > n)
			break ;
		sequence.push_back(next);
		a = b;
		b = next;
	}
	return (sequence);
}
std::vector<size_t> PmergeMe::generateInsertionOrder(size_t pendSize)
{
	if (pendSize == 0)
		return (std::vector<size_t>(0));
	std::vector<size_t> insertionOrder;
	std::vector<size_t> jacobsthal = generateJacobsthalNumbers(pendSize);
	insertionOrder.push_back(0);
	if (pendSize == 1)
		return (insertionOrder);
	size_t	previousJacob = 1;
	for (size_t i = 1; i < jacobsthal.size(); i++)
	{
		size_t	currentJacob = jacobsthal[i];
		if (jacobsthal[i] > pendSize)
			currentJacob = pendSize;
		for (size_t j = currentJacob; j > previousJacob; j--)
			insertionOrder.push_back(j - 1);
		previousJacob = currentJacob;
		if (currentJacob >= pendSize)
			break;
	}
	return (insertionOrder);
}

std::vector<PmergeMe::ElementInfo>	PmergeMe::fordJohnsonSort( std::vector<ElementInfo> &elements)
{
	if (elements.size() <= 1)
		return (elements);
	ElementInfo	straggler;
	bool		hasStraggler = (elements.size() % 2 == 1);
	if (hasStraggler)
	{
		straggler = elements.back();
		elements.pop_back();
	}
	// pair and extract ( larger is in odd position)
	std::vector<ElementInfo> largerElements;
	std::vector<ElementInfo> smallerElements;
	for (size_t i = 0; i < elements.size(); i += 2)
	{
		if (elements[i] > elements[i + 1])
			std::swap(elements[i], elements[i + 1]);
		smallerElements.push_back(elements[i]);
		largerElements.push_back(elements[i + 1]);
	}
	std::vector<ElementInfo> sortedLarger = fordJohnsonSort(largerElements);
	std::vector<ElementInfo> mainChain = sortedLarger;
	std::vector<ElementInfo> pendChain;
	pendChain.reserve(mainChain.size() + hasStraggler);
	// find CORRESPONDING smaller Element (to element in sortedLarger)
	// i believe that this is suboptimal. this should instead use 
	// the originalIndex saved in the element of sortedLarger - 1 to access
	// the corresponding smaller element from elements array (therefore a smallerElements vec is not needed)
	// for (const ElementInfo & largerElem : sortedLarger)
	// 	pendChain.push_back(elements[largerElem.originalIndex - 1]);
	// but this will only be viable if I set the originalIndex anew in every depth level. this needs more thought.
	for (size_t j = 0; j < sortedLarger.size(); j++)
	{
		size_t i = 0;
		while (largerElements[i].value != sortedLarger[j].value
				&& largerElements[i].originalIndex != sortedLarger[j].originalIndex)
		{
			if (i == largerElements.size())
				throw std::logic_error("size Exceeded");
			++i;
		}
		pendChain.push_back(smallerElements[i]);
	}
	if (!pendChain.empty())
	{
		mainChain.insert(mainChain.begin(), pendChain[0]);
		std::vector<size_t> insertionOrder = generateInsertionOrder(pendChain.size());
		for (size_t i = 1; i < insertionOrder.size(); i++)
		{
			size_t	pendIdx = insertionOrder[i];
			// pretty sure this step is not needed, because of how the insertionOrder generation already
			// excludes those values, but one can go sure, unless we just did the loops right here.
			// I think it's very inefficient, although more intuitive to first generate an insertion order.
			// there should just be some nested loops here, that walk backward and so on.
			if (pendIdx >= pendChain.size())
				continue;
			const ElementInfo& elemToInsert = pendChain[pendIdx];
			// find position of large Element in main chain, that is partner to
			// currently to insert small element
			size_t upperBound = mainChain.size();
			for (size_t j = 0; j < mainChain.size(); j++)
			{
				// no need to compare anything else than the value, the element doesn't have to 
				// be unique, if i accidentally have an element before it, that has the same value,
				// it might not be correct in terms of the algorithm, but its actually better for us.
				if (mainChain[j].value == sortedLarger[pendIdx].value
					&& mainChain[j].originalIndex == sortedLarger[pendIdx].originalIndex)
				{
					upperBound = j;
					break;
				}
			}
			//binary search within bounded range
			std::vector<ElementInfo>::iterator insertionPoint = std::lower_bound(
					mainChain.begin(), mainChain.begin() + upperBound,
					elemToInsert);
			mainChain.insert(insertionPoint, elemToInsert);
		}
	}
	if (hasStraggler)
	{
		std::vector<ElementInfo>::iterator insertionPoint = std::lower_bound(
				mainChain.begin(), mainChain.end(),
				straggler);
		mainChain.insert(insertionPoint, straggler);
	}
	return mainChain;
}

void	PmergeMe::mergeInsertionSortVec( std::vector<int> &vec)
{
	if (vec.size() <= 1)
		return;
	std::vector<ElementInfo> elements;
	elements.reserve(vec.size());
	for (size_t i = 0; i < vec.size(); i++)
	{
		ElementInfo e ={vec[i], i};
		elements.push_back(e);
	}
	std::vector<ElementInfo> sorted = fordJohnsonSort(elements);
	vec.clear();
	vec.reserve(sorted.size());
	for (size_t i = 0; i < sorted.size(); i++)
		vec.push_back(sorted[i].value);
}

// void	PmergeMe::insertElementsByGroups(std::vector<ElementInfo>& mainChain, const std::vector<ElementInfo>& pendChain) {
//     if (pendChain.empty()) return;
//     mainChain.insert(mainChain.begin(), pendChain[0]);
// 	if (mainChain.size() == 2)
// 		return ;
//     std::vector<size_t> largeElementPositions(pendChain.size());
//     for (size_t i = 0; i < pendChain.size(); ++i) {
//         // Find position of corresponding large element
//         size_t largeElemPos = i + 1; // +1 because we inserted the first element
//         largeElementPositions[i] = largeElemPos;
//     }
//     // Process remaining elements in groups
//     size_t groupNumber = 1;
//     size_t elementIndex = 1;
//     while (elementIndex < pendChain.size()) {
//         // Calculate current group size
//         size_t groupSize = (groupNumber <= 2) ? 2 : (1U << (groupNumber-2)) + getGroupSize(groupNumber-2);
//         // Ensure we don't go beyond the array bounds
//         groupSize = std::min(groupSize, pendChain.size() - elementIndex);
//         // Process current group (from highest index to lowest)
//         size_t endOfGroup = elementIndex + groupSize;
//         for (size_t i = endOfGroup - 1; i >= elementIndex; --i) {
//             // Get the element to insert
//             const ElementInfo& elemToInsert = pendChain[i];
//             // Find the position of its paired larger element
//             size_t upperBound = largeElementPositions[i];
//             // Binary search with optimized bound
//             std::vector<ElementInfo>::iterator insertPos = std::lower_bound(
//                 mainChain.begin(), mainChain.begin() + upperBound,
//                 elemToInsert);
//             size_t insertIdx = insertPos - mainChain.begin();
//             // Insert the element
//             mainChain.insert(insertPos, elemToInsert);
//             // Update position tracking for all elements after the insertion point
//             for (size_t j = 0; j < pendChain.size(); ++j) {
//                 if (largeElementPositions[j] >= insertIdx) {
//                     largeElementPositions[j]++;
//                 }
//             }
//             if (i == elementIndex) break; // Prevent underflow in unsigned counter
//         }
//         elementIndex += groupSize;
//         groupNumber++;
//     }
// }
//
// std::vector<PmergeMe::ElementInfo>	PmergeMe::FordJohnsonSort( std::vector<ElementInfo> &vec )
// {
// 	if (vec.size() <= 1)
// 		return (vec);
// 	ElementInfo		straggler;
// 	bool	hasStraggler = vec.size() % 2 != 0;
// 	if (hasStraggler)
// 	{
// 		straggler = vec.back();
// 		vec.pop_back();
// 	}
// 	sortInPair(vec);
// 	std::vector<ElementInfo>	pairTable;
// 	pairTable.reserve(vec.size());
// 	int							pairIndex = 0;
// 	for (size_t i = 0; i + 1 < vec.size(); i += 2) {
// 		ElementInfo e1 = { vec[i].value,  pairIndex, vec[i].originalIndex };
// 		ElementInfo e2 = { vec[i+1].value,  pairIndex, vec[i+1].originalIndex };
// 		pairTable.push_back(e1);
// 		pairTable.push_back(e2);
// 		pairIndex++;
// 	}
// 	if (hasStraggler)
// 	{
// 		std::vector<size_t> jacobsthalNumbers = generateJacobsthalNumbers(pairTable.size() + 2);
// 		if (std::find(jacobsthalNumbers.begin(), jacobsthalNumbers.end(), pairTable.size() + 2) != jacobsthalNumbers.end())
// 		{
// 			pairTable.push_back(straggler);
// 			hasStraggler = false;
// 		}
// 	}
// 	std::vector<ElementInfo> pendChain = vectorFromEverySecondElement(pairTable, 0);
// 	std::vector<ElementInfo> largeElements = vectorFromEverySecondElement(pairTable, 1);
// 	std::vector<ElementInfo> indexLookup = FordJohnsonSort( largeElements );
// 	std::vector<ElementInfo> mainChain = buildChain( indexLookup );
// 	// std::vector<ElementInfo> pendChain = buildChain( smallElements );
// 	// generate insertion sequece
// 	// use insertion sequence to insert the current interstion seuence elements smallElement from the large Elements list into mainChain with binary search, with end = current jacob sequence element + 2 (i believe that to be the correct bound but should it up again)
// 	insertElementsByGroups(mainChain, pendChain);
//     if (hasStraggler) {
//         std::vector<ElementInfo>::iterator it = std::lower_bound(
//             mainChain.begin(), mainChain.end(), straggler);
// 		ElementInfo e = { straggler.value, straggler.previousIndex, -1 };
//         mainChain.insert(it, e);
//     }
// 	return (mainChain);
// }
//
// void	PmergeMe::mergeInsertionSortVec( std::vector<int> &vec)
// {
// 	if (vec.size() <= 1)
// 		return ;
// 	std::vector<ElementInfo> vecOfEl;
// 	vecOfEl.reserve(vec.size());
// 	for (size_t i = 0; i < vec.size(); i++)
// 	{
//         ElementInfo e = {vec[i], static_cast<int>(i), static_cast<int>(i)};
// 		vecOfEl.push_back(e);
// 	}
// 	std::vector<ElementInfo> sortedElements = FordJohnsonSort( vecOfEl );
// 	vec.clear();
// 	for (size_t i = 0; i < sortedElements.size(); ++i)
// 		vec.push_back(sortedElements[i].value);
// }
//
// void	PmergeMe::mergeInsertionSortList(std::list<int> &lst)
// {
// 	if (lst.size() <= 1)
// 		return ;
// 	std::list<node> tree;
// 	while ()
// }

/* ====================ElementInfo Operators==================== */

bool PmergeMe::ElementInfo::operator==( const ElementInfo &other ) const
{
	PmergeMe::comparisonCount++;
	return (this->value == other.value);
}

bool PmergeMe::ElementInfo::operator<=( const ElementInfo &other ) const
{
	PmergeMe::comparisonCount++;
	return (this->value <= other.value);
}

bool PmergeMe::ElementInfo::operator>=( const ElementInfo &other ) const
{
	PmergeMe::comparisonCount++;
	return (this->value >= other.value);
}

bool PmergeMe::ElementInfo::operator<( const ElementInfo &other ) const
{
	PmergeMe::comparisonCount++;
	return (this->value < other.value);
}

bool PmergeMe::ElementInfo::operator>( const ElementInfo &other ) const
{
	PmergeMe::comparisonCount++;
	return (this->value > other.value);
}


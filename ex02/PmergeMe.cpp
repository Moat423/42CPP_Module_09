#include "PmergeMe.hpp"
#include "Debug.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

size_t PmergeMe::comparisonCount = 0;
size_t PmergeMe::level = 1;

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

//mergeInsertion with jacobsthal numbers
std::vector<PmergeMe::ElementInfo>	PmergeMe::mergeInsertElements(
		const std::vector<ElementInfo>& lookupSortedSequence, const std::vector<ElementInfo>& pendChain, std::vector<size_t> jacobsthalNumbers)
{
	std::vector<ElementInfo> mainChain = lookupSortedSequence;
	mainChain.insert(mainChain.begin(), pendChain[0]);
	if (pendChain.size() == 1)
		return (mainChain);
	size_t previousJacob = 1;
	for (size_t i = 1; i < pendChain.size(); i++)
	{
		const size_t currentJacob = jacobsthalNumbers[i];
		for (size_t j = currentJacob; j > previousJacob; j--)
		{
			const ElementInfo& elemToInsert = pendChain[j - 1];
			// find position of large Element in main chain, that is partner to
			// currently to insert small element and save it in upperBound
			size_t upperBound = mainChain.size();
			if (j - 1 < lookupSortedSequence.size())
			{
				for (size_t k = 0; k < mainChain.size(); k++)
				{
					if (mainChain[k].value == lookupSortedSequence[j - 1].value
						&& mainChain[k].originalIndex == lookupSortedSequence[j - 1].originalIndex)
					{
						upperBound = k;
						break;
					}
				}
			}
			//binary search within bounded range
			std::vector<ElementInfo>::iterator insertionPoint = std::lower_bound(
					mainChain.begin(), mainChain.begin() + upperBound,
					elemToInsert);
			mainChain.insert(insertionPoint, elemToInsert);
			previousJacob = currentJacob;
		}
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
	ElementInfo	straggler;
	bool		hasStraggler = (elements.size() % 2 == 1);
	if (hasStraggler)
	{
		straggler = elements.back();
		elements.pop_back();
	}
	// pair and extract ( larger is in odd position)
	std::vector<ElementInfo> largerElements;
	for (size_t i = 0; i < elements.size(); i += 2)
	{
		if (elements[i] > elements[i + 1])
			std::swap(elements[i], elements[i + 1]);
		elements[i].previousIndex = elements[i].originalIndex;
		elements[i].originalIndex = i;
		elements[i + 1].previousIndex = elements[i + 1].originalIndex;
		elements[i + 1].originalIndex = i + 1;
		largerElements.push_back(elements[i + 1]);
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
	if (pendChain.size() + 1 == jacobsthalNumbers.back())
	{
		hasStraggler = false;
		pendChain.push_back(straggler);
	}
	std::vector<ElementInfo> mainChain = mergeInsertElements(
			sortedLarger, pendChain, jacobsthalNumbers);
	if (hasStraggler)
	{
		std::vector<ElementInfo>::iterator insertionPoint = std::lower_bound(
				mainChain.begin(), mainChain.end(),
				straggler);
		mainChain.insert(insertionPoint, straggler);
	}
	std::cout << "after insertion, before returning: "  << std::endl;
	printContainer(mainChain);
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
		ElementInfo e ={vec[i], i, std::numeric_limits<size_t>::max()};
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

std::ostream& operator<<(std::ostream& os, const PmergeMe::ElementInfo& info) {
    os << "[val: " << info.value 
       << ", origIdx: " << info.originalIndex 
       << ", prevIdx: " << info.previousIndex << "]";
    return os;
}

PmergeMe::ElementInfo& PmergeMe::ElementInfo::operator=( const ElementInfo &assign )
{
	this->value = assign.value;
	this->originalIndex = assign.originalIndex;
	this->previousIndex = assign.previousIndex;
	return (*this);
}

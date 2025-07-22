#include "PmergeMe.hpp"
#include "Debug.hpp"
#include <algorithm>
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
std::vector<PmergeMe::ElementInfo>	PmergeMe::sortInPair( std::vector<ElementInfo> &vec )
{
	std::vector<ElementInfo> originalPositions;
	for (size_t i = 0; i < vec.size() - 1; i += 2)
	{
		if (vec[i] < vec [i + 1])
			std::swap(vec[i], vec[i + 1]);
		originalPositions.push_back(vec[i]);
		originalPositions.push_back(vec[i + 1]);
	}
	if (vec.size() % 2 != 0)
			originalPositions.push_back(vec[vec.size() - 1]);
	if (originalPositions.size() != vec.size())
		throw std::logic_error("originalPositions and vec size in sort in pair not same");
	return (originalPositions);
}

// takes either every (i*2 -n)th elemement
std::vector<int>	PmergeMe::vectorFromEverySecond( const std::vector<int> vec , int n)
{
	if (n != 0 && n != 1)
		return (std::vector<int>());
	std::vector<int>	Elements;
	for (size_t i = n; i < vec.size(); i += 2)
		Elements.push_back(vec[i]);
	return (Elements);
}

// takes either every (i*2 -n)th elemement
std::vector<PmergeMe::ElementInfo>	PmergeMe::vectorFromEverySecondElement( const std::vector<ElementInfo> vec , int n)
{
	if (n != 0 && n != 1)
		return (std::vector<ElementInfo>());
	std::vector<ElementInfo>	Elements;
	Elements.reserve(vec.size() / 2);
	for (size_t i = n; i < vec.size(); i += 2)
		Elements.push_back(vec[i]);
	return (Elements);
}

PmergeMe::ElementInfo	PmergeMe::getElementFromTableForPrevious(int index, const std::vector<ElementInfo> table)
{
	ElementInfo	e = {table[index].value, table[index].previousIndex, -1};
	return (e);
}

// build chain from elements, to
// make a duplicate of largeElements or smallElements, but with previousIndex set to -1 and original
// index to what was prevIndex in largeElements
std::vector<PmergeMe::ElementInfo>	PmergeMe::buildChain( const std::vector<ElementInfo> &elementsList )
{
	std::vector<ElementInfo>	chain;
	for (size_t i = 0; i < elementsList.size(); i++)
	{
		ElementInfo e = { elementsList[i].value, elementsList[i].originalIndex, -1 };
		chain.push_back(e);
	}
	return (chain);
}

std::vector<size_t>	PmergeMe::generateJacobsthalNumbers(size_t n)
{
	std::vector<size_t>	sequence;

	sequence.push_back(0);
	sequence.push_back(1);
	for (size_t i = 2; i < n; i++)
	{
		int next = sequence[i - 1] + 2 * sequence[i - 2];
		sequence.push_back(next);
	}
	return (sequence);
}

size_t getGroupSize(size_t n) {
    if (n <= 2) return 2;
    return (1U << (n-2)) + getGroupSize(n-2);
}

void	PmergeMe::insertElementsByGroups(std::vector<ElementInfo>& mainChain, const std::vector<ElementInfo>& pendChain) {
    if (pendChain.empty()) return;
    // Always insert the first element at position 0
    mainChain.insert(mainChain.begin(), pendChain[0]);
	if (mainChain.size() == 2)
		return ;
    // Keep track of position adjustments as we insert elements
    std::vector<size_t> largeElementPositions(pendChain.size());
    for (size_t i = 0; i < pendChain.size(); ++i) {
        // Find position of corresponding large element
        size_t largeElemPos = i + 1; // +1 because we inserted the first element
        largeElementPositions[i] = largeElemPos;
    }
    // Process remaining elements in groups
    size_t groupNumber = 1; // Start with the first group
    size_t elementIndex = 1; // Skip the first element (already inserted)
    while (elementIndex < pendChain.size()) {
        // Calculate current group size
        size_t groupSize = (groupNumber <= 2) ? 2 : (1U << (groupNumber-2)) + getGroupSize(groupNumber-2);
        // Ensure we don't go beyond the array bounds
        groupSize = std::min(groupSize, pendChain.size() - elementIndex);
        // Process current group (from highest index to lowest)
        size_t endOfGroup = elementIndex + groupSize;
        for (size_t i = endOfGroup - 1; i >= elementIndex; --i) {
            // Get the element to insert
            const ElementInfo& elemToInsert = pendChain[i];
            // Find the position of its paired larger element
            size_t upperBound = largeElementPositions[i];
            // Binary search with optimized bound
            std::vector<ElementInfo>::iterator insertPos = std::lower_bound(
                mainChain.begin(), mainChain.begin() + upperBound,
                elemToInsert);
            size_t insertIdx = insertPos - mainChain.begin();
            // Insert the element
            mainChain.insert(insertPos, elemToInsert);
            // Update position tracking for all elements after the insertion point
            for (size_t j = 0; j < pendChain.size(); ++j) {
                if (largeElementPositions[j] >= insertIdx) {
                    largeElementPositions[j]++;
                }
            }
            if (i == elementIndex) break; // Prevent underflow in unsigned counter
        }
        elementIndex += groupSize;
        groupNumber++;
    }
}

std::vector<PmergeMe::ElementInfo>	PmergeMe::FordJohnsonSort( std::vector<ElementInfo> &vec )
{
	if (vec.size() <= 1)
		return (vec);
	ElementInfo		straggler;
	bool	hasStraggler = vec.size() % 2 != 0;
	if (hasStraggler)
	{
		straggler = vec.back();
		vec.pop_back();
	}
	std::vector<ElementInfo>			originalPositions = sortInPair(vec);
	std::vector<ElementInfo>	pairTable;
	pairTable.reserve(vec.size());
	int							pairIndex = 0;
	for (size_t i = 0; i + 1 < originalPositions.size(); i += 2) {
		ElementInfo e1 = { originalPositions[i].value,  pairIndex, originalPositions[i].originalIndex };
		ElementInfo e2 = { originalPositions[i+1].value,  pairIndex, originalPositions[i+1].originalIndex };
		pairTable.push_back(e1);
		pairTable.push_back(e2);
		pairIndex++;
	}
	// if (hasStraggler)
	// {
	// 	ElementInfo e = { straggler.value, -1, straggler.originalIndex };
	// 	pairTable.push_back(e);
	// }
	std::vector<ElementInfo> smallElements = vectorFromEverySecondElement(pairTable, 1);
	std::vector<ElementInfo> largeElements = vectorFromEverySecondElement(pairTable, 0);
	std::vector<ElementInfo> indexLookup = FordJohnsonSort( largeElements );
	std::vector<ElementInfo> mainChain = buildChain( indexLookup );
	std::vector<ElementInfo> pendChain = buildChain( smallElements );
	// generate insertion sequece
	// use insertion sequence to insert the current interstion seuence elements smallElement from the large Elements list into mainChain with binary search, with end = current jacob sequence element + 2 (i believe that to be the correct bound but should it up again)
	insertElementsByGroups(mainChain, pendChain);
    if (hasStraggler) {
        std::vector<ElementInfo>::iterator it = std::lower_bound(
            mainChain.begin(), mainChain.end(), straggler);
        mainChain.insert(it, straggler);
    }
	return (mainChain);
}

void	PmergeMe::mergeInsertionSortVec( std::vector<int> &vec)
{
	if (vec.size() <= 1)
		return ;
	std::vector<ElementInfo> vecOfEl;
	for (size_t i = 0; i < vec.size(); i++)
	{
        ElementInfo e = {vec[i], static_cast<int>(i), static_cast<int>(i)};
		vecOfEl.push_back(e);
	}
	std::vector<ElementInfo> sortedElements = FordJohnsonSort( vecOfEl );
	vec.clear();
	for (size_t i = 0; i < sortedElements.size(); ++i)
	{
		vec.push_back(sortedElements[i].value);
	}
}

/* ====================ElementInfo Operators==================== */

bool PmergeMe::ElementInfo::operator<( const ElementInfo &other ) const
{
	PmergeMe::comparisonCount++;
	return (this->value < other.value);
}

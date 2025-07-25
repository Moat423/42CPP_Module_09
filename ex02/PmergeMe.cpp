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
void	PmergeMe::sortInPair( std::vector<ElementInfo> &vec )
{
	for (size_t i = 0; i < vec.size() - 1; i += 2)
		if (vec[i] < vec [i + 1])
			std::swap(vec[i], vec[i + 1]);
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

// build chain from elements, to
// make a duplicate of largeElements or smallElements, but with previousIndex set to -1 and original
// index to what was prevIndex in largeElements
std::vector<PmergeMe::ElementInfo>	PmergeMe::buildChain( const std::vector<ElementInfo> &elementsList )
{
	std::vector<ElementInfo>	chain;
	chain.reserve(elementsList.size());
	for (size_t i = 0; i < elementsList.size(); i++)
	{
		ElementInfo e = { elementsList[i].value, elementsList[i].previousIndex, -1 };
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
//
// void	PmergeMe::insertElements(std::vector<ElementInfo>& mainChain, const std::vector<ElementInfo>& pendChain, const std::vector<int> jacobsthalNumbers)
// {
// 	if (pendChain.empty()) return;
// 	mainChain.insert(mainChain.begin(), pendChain[0]);
// 	if (pendChain.size() == 1)
// 		return ;
// }

void	PmergeMe::insertElementsByGroups(std::vector<ElementInfo>& mainChain, const std::vector<ElementInfo>& pendChain) {
    if (pendChain.empty()) return;
    mainChain.insert(mainChain.begin(), pendChain[0]);
	if (mainChain.size() == 2)
		return ;
    std::vector<size_t> largeElementPositions(pendChain.size());
    for (size_t i = 0; i < pendChain.size(); ++i) {
        // Find position of corresponding large element
        size_t largeElemPos = i + 1; // +1 because we inserted the first element
        largeElementPositions[i] = largeElemPos;
    }
    // Process remaining elements in groups
    size_t groupNumber = 1;
    size_t elementIndex = 1;
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
	sortInPair(vec);
	std::vector<ElementInfo>	pairTable;
	pairTable.reserve(vec.size());
	int							pairIndex = 0;
	for (size_t i = 0; i + 1 < vec.size(); i += 2) {
		ElementInfo e1 = { vec[i].value,  pairIndex, vec[i].originalIndex };
		ElementInfo e2 = { vec[i+1].value,  pairIndex, vec[i+1].originalIndex };
		pairTable.push_back(e1);
		pairTable.push_back(e2);
		pairIndex++;
	}
	if (hasStraggler)
	{
		std::vector<size_t> jacobsthalNumbers = generateJacobsthalNumbers(pairTable.size() + 2);
		if (std::find(jacobsthalNumbers.begin(), jacobsthalNumbers.end(), pairTable.size() + 2) != jacobsthalNumbers.end())
		{
			pairTable.push_back(straggler);
			hasStraggler = false;
		}
	}
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
		ElementInfo e = { straggler.value, straggler.previousIndex, -1 };
        mainChain.insert(it, e);
    }
	return (mainChain);
}

void	PmergeMe::mergeInsertionSortVec( std::vector<int> &vec)
{
	if (vec.size() <= 1)
		return ;
	std::vector<ElementInfo> vecOfEl;
	vecOfEl.reserve(vec.size());
	for (size_t i = 0; i < vec.size(); i++)
	{
        ElementInfo e = {vec[i], static_cast<int>(i), static_cast<int>(i)};
		vecOfEl.push_back(e);
	}
	std::vector<ElementInfo> sortedElements = FordJohnsonSort( vecOfEl );
	vec.clear();
	for (size_t i = 0; i < sortedElements.size(); ++i)
		vec.push_back(sortedElements[i].value);
}

/* ====================ElementInfo Operators==================== */

bool PmergeMe::ElementInfo::operator<( const ElementInfo &other ) const
{
	PmergeMe::comparisonCount++;
	return (this->value < other.value);
}

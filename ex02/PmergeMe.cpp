#include "PmergeMe.hpp"
#include "Debug.hpp"
#include "string"
#include <algorithm>
#include <stdexcept>
#include <vector>

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
	std::vector<int> originalPositions;
	for (int i = 1; i < vec.size(); i += 2)
	{
		if (vec[i] < vec [i - 1])
		{
			std::swap(vec[i], vec[i - 1]);
			originalPositions.push_back(i + 1);
			originalPositions.push_back(i);
			continue;
		}
		originalPositions.push_back(i);
		originalPositions.push_back(i + 1);
	}
	if (originalPositions.size() != vec.size())
		throw std::logic_error("originalPositions and vec size in sort in pair not same");
	return (originalPositions);
}

// takes either every (i*2 -n)th elemement
std::vector<int>	PmergeMe::vectorFromEverySecond( const std::vector<int> vec , int n)
{
	if (n != 0 || n != 1)
		return (std::vector<int>());
	std::vector<int>	Elements;
	for (int i = n; i < vec.size(); i += 2)
		Elements.push_back(vec[i]);
}

// takes either every (i*2 -n)th elemement
std::vector<PmergeMe::ElementInfo>	PmergeMe::vectorFromEverySecondElement( const std::vector<ElementInfo> vec , int n)
{
	if (n != 0 || n != 1)
		return (std::vector<ElementInfo>());
	std::vector<ElementInfo>	Elements;
	for (int i = n; i < vec.size(); i += 2)
		Elements.push_back(vec[i]);
}

PmergeMe::ElementInfo	PmergeMe::getElementFromTableForPrevious(int index, const std::vector<ElementInfo> table)
{
	ElementInfo	e = {table[index].value, table[index].previousIndex, -1};
}

// build chain from elements, to
// make a duplicate of largeElements or smallElements, but with previousIndex set to -1 and original
// index to what was prevIndex in largeElements
std::vector<PmergeMe::ElementInfo>	PmergeMe::buildChain( const std::vector<ElementInfo> &ElementsList )
{
	std::vector<ElementInfo>	chain;
	for (size_t i = 0; i < ElementsList.size(); i++)
	{
		ElementInfo e = { ElementsList[i].value, ElementsList[i].originalIndex, -1 };
		chain.push_back(e);
	}
	return (chain);
}

std::vector<PmergeMe::ElementInfo>	PmergeMe::FordJohnsonSort( std::vector<ElementInfo> &vec )
{
	if (vec.size() <= 1)
		return (std::vector<ElementInfo>());
	ElementInfo		straggler;
	bool	hasStraggler = vec.size() % 2 != 0;
	if (hasStraggler)
	{
		straggler = vec.back();
		vec.pop_back();
	}
	std::vector<ElementInfo>			originalPositions = sortInPair(vec);
	std::vector<ElementInfo>	pairTable;
	int							pairIndex = 0;
	for (size_t i = 0; i + 1 < originalPositions.size(); i += 2) {
		ElementInfo e1 = { originalPositions[i].value,  pairIndex, originalPositions[i].originalIndex };
		ElementInfo e2 = { originalPositions[i+1].value,  pairIndex, originalPositions[i+1].originalIndex };
		pairTable.push_back(e1);
		pairTable.push_back(e2);
		pairIndex++;
	}
	if (hasStraggler)
	{
		ElementInfo e = { straggler.value, -1, straggler.originalIndex };
		pairTable.push_back(e);
	}
	std::vector<ElementInfo> smallElements = vectorFromEverySecondElement(pairTable, 1);
	std::vector<ElementInfo> largeElements = vectorFromEverySecondElement(pairTable, 0);
	std::vector<ElementInfo> indexLookup = FordJohnsonSort( largeElements );
	std::vector<ElementInfo> mainChain = buildChain( largeElements );
	std::vector<ElementInfo> pendChain = buildChain( smallElements );
	// generate insertion sequece
	// use insertion sequence to insert the current interstion seuence elements smallElement from the large Elements list into mainChain with binary search, with end = current jacob sequence element + 2 (i believe that to be the correct bound but should it up again)

}


void	PmergeMe::mergeInsertionSortVec( std::vector<int> &vec)
{
	if (vec.size() <= 1)
		return ;
	int		straggler = -1;
	bool	hasStraggler = vec.size() % 2 != 0;
	if (hasStraggler)
	{
		straggler = vec.back();
		vec.pop_back();
	}
	std::vector<int> originalPositions = sortInPair(vec);
	std::vector<int> largeElements = vectorFromEverySecond(vec, 0);
	std::vector<int> indexLookup = FordJohnsonSort( largeElements );

	while (/*largeElements.size > 2 */)
	{
	//pair up
	//sort in pair (small in front, create lookup table,)
	//vec of largerElements
	}
	//pair up
	//sort in pair (small in front)
	//mergeInsert
}

/* ====================ElementInfo Operators==================== */

bool PmergeMe::ElementInfo::operator==( const ElementInfo &other ) const
{
	return (this->value == other.value);
}

bool PmergeMe::ElementInfo::operator>( const ElementInfo &other ) const
{
	return (this->value > other.value);
}

bool PmergeMe::ElementInfo::operator<( const ElementInfo &other ) const
{
	return (this->value < other.value);
}

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
std::vector<int>	PmergeMe::sortInPair( std::vector<int> &vec )
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

std::vector<int>	PmergeMe::FordJohnsonSort( std::vector<int> &vec )
{
	if (vec.size() <= 1)
		return (std::vector<int>());
	int		straggler = -1;
	bool	hasStraggler = vec.size() % 2 != 0;
	if (hasStraggler)
	{
		straggler = vec.back();
		vec.pop_back();
	}
	std::vector<int> originalPositions = sortInPair(vec);
	std::vector<int> smallElements = vectorFromEverySecond(vec, 1);
	std::vector<int> largeElements = vectorFromEverySecond(vec, 0);
	std::vector<int> indexLookup = FordJohnsonSort( largeElements );
	std::vector<int> mainChain(largeElements);
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

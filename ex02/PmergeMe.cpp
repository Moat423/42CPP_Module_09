#include "PmergeMe.hpp"
#include "Debug.hpp"
#include "string"
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
	return *this;
}

void	PmergeMe::mergeInsertionSortVec( std::vector<int> &vec)
{
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

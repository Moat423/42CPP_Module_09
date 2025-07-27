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

std::vector<size_t>	PmergeMe::generateJacobsthalNumbers(size_t tillAtLeast)
{
	std::vector<size_t>	sequence;
	sequence.reserve(tillAtLeast);
	sequence.push_back(1);
	if (tillAtLeast == 1)
		return (sequence);
	sequence.push_back(1);
	size_t	a = 1;
	size_t	b = 1;
	while (1)
	{
		size_t next = b + 2 * a;
		sequence.push_back(next);
		if (next > tillAtLeast)
			break ;
		a = b;
		b = next;
	}
	return (sequence);
}

//mergeInsertion with jacobsthal numbers
std::vector<PmergeMe::ElementInfo>	PmergeMe::mergeInsertElements(
		const std::vector<ElementInfo>& lookupSortedSequence, const std::vector<ElementInfo>& pendChain, std::vector<size_t> jacobsthalNumbers)
{
	std::vector<ElementInfo> mainChain = lookupSortedSequence;
	// always insert first element of pendChain at the beginning of mainChain, because first element in pendChain is smaller partner to first element in mainChain
	mainChain.insert(mainChain.begin(), pendChain[0]);
	if (pendChain.size() == 1)
		return (mainChain);
	size_t previousJacob = 0;
	for (size_t jacobsIndex = 2; jacobsIndex < jacobsthalNumbers.size(); jacobsIndex++)
	{
		size_t currentJacob = (jacobsthalNumbers[jacobsIndex] - 1 ) < pendChain.size() - 1
			? (jacobsthalNumbers[jacobsIndex] - 1) : pendChain.size() - 1;
		for (size_t pendChainIndexToInsert = currentJacob; pendChainIndexToInsert > previousJacob; pendChainIndexToInsert--)
		{
			const ElementInfo& elemToInsert = pendChain[pendChainIndexToInsert];
			// inserting an element from pendChain into mainChain. if pendChainIndexToInsert is out of bounds, it is a straggler
			size_t bound = mainChain.size();
			if (pendChainIndexToInsert < lookupSortedSequence.size())
			{
				// have to know at what point the elements to inserts partner is in main chain, so i only search up to that point.
				// can make this better, by making vec of previous insertion points and adding them up to pendChainIndexToInsert if they are lower step by step
				for (size_t k = 0; k < mainChain.size(); k++)
				{
					if (lookupSortedSequence[pendChainIndexToInsert].value == mainChain[k].value)
					{
						bound = k;
						break;
					}
				}
			}
			//binary search within bounded range
			std::vector<ElementInfo>::iterator insertionPoint = std::lower_bound(
					mainChain.begin(), mainChain.begin() + bound,
					elemToInsert);
			mainChain.insert(insertionPoint, elemToInsert);
		}
		previousJacob = currentJacob;
	}
	return (mainChain);
}


std::vector<PmergeMe::ElementInfo>	PmergeMe::fordJohnsonSort( std::vector<ElementInfo> &vec)
{
	std::vector<ElementInfo> elements = vec;
	if (vec.size() <= 1)
	{
		elements[0].previousIndex = elements[0].originalIndex;
		elements[0].originalIndex = 0;
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
	// take care of straggler, if one element can't be paired
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
	}
	if (elements.size() / 2 != largerElements.size())
		throw std::logic_error("Size mismatch");
	std::vector<ElementInfo> sortedLarger = fordJohnsonSort(largerElements);

	std::vector<ElementInfo> pendChain;
	pendChain.reserve(sortedLarger.size() + (1 & hasStraggler));
	for (size_t i = 0; i < sortedLarger.size(); i++)
	{
		sortedLarger[i] = (elements[sortedLarger[i].previousIndex]);
		pendChain.push_back(elements[sortedLarger[i].originalIndex - 1]);
	}
	if (pendChain.size() == 0)
		throw std::logic_error("pendChain is empty, but should not be");
	std::vector<size_t> jacobsthalNumbers = generateJacobsthalNumbers(pendChain.size() + 2);
	// if straggler index is present in jacobsthal numbers, can insert it in regular mergeInsertElements, else do insertion at end
	if (hasStraggler && pendChain.size() == jacobsthalNumbers.back() - 1)
	{
		hasStraggler = false;
		pendChain.push_back(straggler);
	}
	std::vector<ElementInfo> mainChain = mergeInsertElements(
			sortedLarger, pendChain, jacobsthalNumbers);
	// if straggler could not be inserted earlier, do now
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
		ElementInfo e ={vec[i], i, std::numeric_limits<size_t>::max()};
		elements.push_back(e);
	}
	std::vector<ElementInfo> sorted = fordJohnsonSort(elements);
	vec.clear();
	vec.reserve(sorted.size());
	for (size_t i = 0; i < sorted.size(); i++)
		vec.push_back(sorted[i].value);
}

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

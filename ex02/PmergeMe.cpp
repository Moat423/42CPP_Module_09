#include "PmergeMe.hpp"
#include "Debug.hpp"
#include <algorithm>
#include <deque>
#include <limits>
#include <vector>

size_t PmergeMe::comparisonCount = 0;
size_t PmergeMe::level = 1;

// Default Constructor
PmergeMe::PmergeMe( void )
{
	debug("Default Constructor called");
}

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

const std::deque<size_t>	PmergeMe::generateJacobsthalNumbersDeque(size_t tillAtLeast)
{
	std::deque<size_t>	sequence;
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

std::deque<PmergeMe::ElementInfo>	PmergeMe::mergeInsertElementsDeque(
		const std::deque<ElementInfo>& lookupSortedSequence, const std::deque<ElementInfo>& pendChain, std::deque<size_t> jacobsthalNumbers)
{
	std::deque<ElementInfo> mainChain = lookupSortedSequence;
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
			size_t bound = mainChain.size();
			if (pendChainIndexToInsert < lookupSortedSequence.size())
			{
				for (size_t k = 0; k < mainChain.size(); k++)
				{
					if (lookupSortedSequence[pendChainIndexToInsert].value == mainChain[k].value)
					{
						bound = k;
						break;
					}
				}
			}
			std::deque<ElementInfo>::iterator insertionPoint = std::lower_bound(
					mainChain.begin(), mainChain.begin() + bound,
					elemToInsert);
			mainChain.insert(insertionPoint, elemToInsert);
		}
		previousJacob = currentJacob;
	}
	return (mainChain);
}

std::deque<PmergeMe::ElementInfo>	PmergeMe::pairAndExtractDeque( std::deque<ElementInfo> &elements)
{
	std::deque<ElementInfo> largerElements;
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
	return largerElements;
}

std::deque<PmergeMe::ElementInfo>	PmergeMe::fordJohnsonSortDeque(const std::deque<ElementInfo> &vec)
{
	std::deque<ElementInfo> elements = vec;
	if (elements.size() <= 1)
	{
		elements[0].previousIndex = elements[0].originalIndex;
		elements[0].originalIndex = 0;
		return (elements);
	}
	std::deque<ElementInfo> largerElements = pairAndExtractDeque(elements);
	ElementInfo	straggler;
	bool		hasStraggler = (elements.size() % 2 == 1);
	if (hasStraggler)
	{
		straggler.value = elements.back().value;
		straggler.originalIndex = std::numeric_limits<size_t>::max();
		straggler.previousIndex = elements.back().originalIndex;
		elements.pop_back();
	}
	std::deque<ElementInfo> sortedLarger = fordJohnsonSortDeque(largerElements);
	std::deque<ElementInfo> pendChain;
	for (size_t i = 0; i < sortedLarger.size(); i++)
	{
		sortedLarger[i] = (elements[sortedLarger[i].previousIndex]);
		pendChain.push_back(elements[sortedLarger[i].originalIndex - 1]);
	}
	const std::deque<size_t> jacobsthalNumbers = generateJacobsthalNumbersDeque(pendChain.size() + 2);
	if (hasStraggler && pendChain.size() == jacobsthalNumbers.back() - 1)
	{
		hasStraggler = false;
		pendChain.push_back(straggler);
	}
	std::deque<ElementInfo> mainChain = mergeInsertElementsDeque(
			sortedLarger, pendChain, jacobsthalNumbers);
	if (hasStraggler)
	{
		std::deque<ElementInfo>::iterator insertionPoint = std::lower_bound(
				mainChain.begin(), mainChain.end(),
				straggler);
		mainChain.insert(insertionPoint, straggler);
	}
	return mainChain;
}

void	PmergeMe::mergeInsertionSortDeq( std::deque<int> &vec)
{
	if (vec.size() <= 1)
		return;
	std::deque<ElementInfo> elements;
	for (size_t i = 0; i < vec.size(); i++)
	{
		ElementInfo e ={vec[i], i, std::numeric_limits<size_t>::max()};
		elements.push_back(e);
	}
	std::deque<ElementInfo> sorted = fordJohnsonSortDeque(elements);
	vec.clear();
	for (size_t i = 0; i < sorted.size(); i++)
		vec.push_back(sorted[i].value);
}

const std::vector<size_t>	PmergeMe::generateJacobsthalNumbers(size_t tillAtLeast)
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
	std::vector<std::vector<ElementInfo>::iterator> allInsertionPoints;
	allInsertionPoints.reserve(pendChain.size() - 1);
	for (size_t jacobsIndex = 2; jacobsIndex < jacobsthalNumbers.size(); jacobsIndex++)
	{
		size_t currentJacob = (jacobsthalNumbers[jacobsIndex] - 1 ) < pendChain.size() - 1
			? (jacobsthalNumbers[jacobsIndex] - 1) : pendChain.size() - 1;
		for (size_t pendChainIndexToInsert = currentJacob; pendChainIndexToInsert > previousJacob; pendChainIndexToInsert--)
		{
			const ElementInfo& elemToInsert = pendChain[pendChainIndexToInsert];
			// inserting an element from pendChain into mainChain. if pendChainIndexToInsert is out of bounds, it is a straggler
			allInsertionPoints.push_back(mainChain.end());
			if (pendChainIndexToInsert < lookupSortedSequence.size())
			{
				allInsertionPoints.back() = mainChain.begin() + pendChainIndexToInsert + 1;
				for (std::vector<ElementInfo>::iterator InsertionPointIt = mainChain.begin(); InsertionPointIt != mainChain.end() - 1 ; InsertionPointIt++)
					allInsertionPoints.back() += 1 & (*InsertionPointIt < *allInsertionPoints.back());
			}
			//binary search within bounded range
			std::vector<ElementInfo>::iterator insertionPoint = std::lower_bound(
					mainChain.begin(), allInsertionPoints.back(),
					elemToInsert);
			mainChain.insert(insertionPoint, elemToInsert);
			allInsertionPoints.push_back(insertionPoint);
		}
		previousJacob = currentJacob;
	}
	return (mainChain);
}

std::vector<PmergeMe::ElementInfo>	PmergeMe::pairAndExtract( std::vector<ElementInfo> &elements)
{
	std::vector<ElementInfo> largerElements;
	largerElements.reserve(elements.size() / 2 + 1);
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
	return largerElements;
}

std::vector<PmergeMe::ElementInfo>	PmergeMe::fordJohnsonSort(const std::vector<ElementInfo> &vec)
{
	std::vector<ElementInfo> elements = vec;
	if (elements.size() <= 1)
	{
		elements[0].previousIndex = elements[0].originalIndex;
		elements[0].originalIndex = 0;
		return (elements);
	}
	std::vector<ElementInfo> largerElements = pairAndExtract(elements);
	// take care of straggler, if one element can't be paired
	ElementInfo	straggler;
	bool		hasStraggler = (elements.size() % 2 == 1);
	if (hasStraggler)
	{
		straggler.value = elements.back().value;
		straggler.originalIndex = std::numeric_limits<size_t>::max();
		straggler.previousIndex = elements.back().originalIndex;
		elements.pop_back();
	}
	//recursively sort the larger elements
	std::vector<ElementInfo> sortedLarger = fordJohnsonSort(largerElements);
	std::vector<ElementInfo> pendChain;
	pendChain.reserve(sortedLarger.size() + (1 & hasStraggler));
	for (size_t i = 0; i < sortedLarger.size(); i++)
	{
		sortedLarger[i] = (elements[sortedLarger[i].previousIndex]);
		pendChain.push_back(elements[sortedLarger[i].originalIndex - 1]);
	}
	const std::vector<size_t> jacobsthalNumbers = generateJacobsthalNumbers(pendChain.size() + 2);
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

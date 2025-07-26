#ifndef PMERGEME_HPP
# define PMERGEME_HPP
#include <ctime>
#include <vector>
#include <list>
#include <deque>
#include <iostream>

// give back the time taken in seconds
template<typename Func, typename Container>
double	stopwatch(Func func, Container &functionArgument)
{
	std::clock_t cStart = std::clock();
	func(functionArgument);
	std::clock_t cEnd = std::clock();
	return (static_cast<double>(cEnd - cStart) / CLOCKS_PER_SEC);
}

template<typename Container>
void	printContainer(Container container)
{
	typename Container::iterator it = container.begin();
	if (container.size() == 0)
	{
		std::cout << "empty" << std::endl;
		return ;
	}
	while (it != container.end() - 1)
		std::cout << *it++ << " ";
	std::cout << *it << std::endl;
}

template<typename Iterator>
bool is_sorted(Iterator first, Iterator last) {
    if (first == last) return true;
    Iterator next = first;
    ++next;
    for (; next != last; ++first, ++next) {
        if (*next < *first)
            return false;
    }
    return true;
}

class PmergeMe {
	public:
		//contructors and destructors
		PmergeMe( void );
		// PmergeMe( std::string );
		virtual ~PmergeMe();
		PmergeMe(const PmergeMe &copy);
		//operator overloads
		PmergeMe& operator=( const PmergeMe &assign );
		//methods
		static void	mergeInsertionSortVec( std::vector<int> &vec);

		//variables
		static size_t	comparisonCount;
		static size_t	level;
	// LIST
		void	mergeInsertionSortList(std::list<int> &lst);
		typedef struct ElementInfo
		{
			int	value;
			size_t	originalIndex;
			size_t	previousIndex;
			// int	previousIndex;
			ElementInfo& operator=( const ElementInfo &assign );
			bool operator<( const ElementInfo &other ) const;
			bool operator==( const PmergeMe::ElementInfo &other ) const;
			bool operator<=( const PmergeMe::ElementInfo &other ) const;
			bool operator>=( const PmergeMe::ElementInfo &other ) const;
			bool operator>( const PmergeMe::ElementInfo &other ) const;
		} ElementInfo;
	private:
		static std::vector<size_t> generateInsertionOrder(size_t pendSize);
		static std::vector<ElementInfo>	fordJohnsonSort( std::vector<ElementInfo> &vec );
		static std::vector<PmergeMe::ElementInfo>	mergeInsertElements(
				const std::vector<ElementInfo>& lookupSortedSequence, const std::vector<ElementInfo>& pendChain, std::vector<size_t> JacobsthalNumbers);

		static void	insertElements(std::vector<ElementInfo>& mainChain, const std::vector<ElementInfo>& pendChain, const std::vector<int> jacobsthalNumbers);
		static void	sortInPair( std::vector<ElementInfo> &vec );
		static PmergeMe::ElementInfo	IntToElementInfo(int value, int index);
		static std::vector<PmergeMe::ElementInfo>	vectorFromEverySecondElement( const std::vector<ElementInfo> vec , int n);
		static std::vector<PmergeMe::ElementInfo>	FordJohnsonSort( std::vector<ElementInfo> &vec );
		static std::vector<PmergeMe::ElementInfo>	buildChain( const std::vector<ElementInfo> &largeElements );
		static std::vector<int>	FordJohnsonSort( std::vector<int> &vec );
		static std::vector<int>	pair( std::vector<int> &vec );
		static std::vector<size_t>	generateJacobsthalNumbers(size_t n);
		static void	insertElementsByGroups(std::vector<ElementInfo>& mainChain, const std::vector<ElementInfo>& pendChain);
	// LIST
		typedef struct node
		{
			int value;
			struct node	*prev;
		} node ;
};

std::ostream& operator<<(std::ostream& os, const PmergeMe::ElementInfo& info);

#endif // !PMERGEME_HPP


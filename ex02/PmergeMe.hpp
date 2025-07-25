#ifndef PMERGEME_HPP
# define PMERGEME_HPP
#include <ctime>
#include <vector>
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
		PmergeMe( void );
		// PmergeMe( std::string );
		virtual ~PmergeMe();

		PmergeMe(const PmergeMe &copy);
		PmergeMe& operator=( const PmergeMe &assign );
		static void	mergeInsertionSortVec( std::vector<int> &vec);

		static size_t	comparisonCount;
	private:
		typedef struct ElementInfo
		{
			int	value;
			int	originalIndex;
			int	previousIndex;
			bool operator<( const ElementInfo &other ) const;
		} ElementInfo;
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
};


#endif // !PMERGEME_HPP


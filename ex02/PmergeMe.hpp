#ifndef PMERGEME_HPP
# define PMERGEME_HPP
#include <ctime>
#include <vector>
#include <deque>
#include <iostream>

template<typename Func, typename Container>
double	stopwatch(Func func, Container functionArgument)
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
	while (it != container.end() - 1)
		std::cout << *it++ << " ";
	std::cout << *it << std::endl;
}

class PmergeMe {
	public:
		PmergeMe( void );
		// PmergeMe( std::string );
		virtual ~PmergeMe();

		PmergeMe(const PmergeMe &copy);
		PmergeMe& operator=( const PmergeMe &assign );
		void	mergeInsertionSortVec( std::vector<int> &vec);

	private:
		typedef struct ElementInfo
		{
			int	value;
			int	originalIndex;
			int	previousIndex;
			bool operator==( const ElementInfo &other ) const;
			bool operator>( const ElementInfo &other ) const;
			bool operator<( const ElementInfo &other ) const;
		} ElementInfo;
		// std::vector<int>	sortInPair( std::vector<int> &vec );
		std::vector<PmergeMe::ElementInfo>	sortInPair( std::vector<ElementInfo> &vec );
		std::vector<int>	vectorFromEverySecond( const std::vector<int> vec , int n);
		std::vector<PmergeMe::ElementInfo>	vectorFromEverySecondElement( const std::vector<ElementInfo> vec , int n);
		std::vector<PmergeMe::ElementInfo>	FordJohnsonSort( std::vector<ElementInfo> &vec );
		PmergeMe::ElementInfo	getElementFromTableForPrevious(int index, const std::vector<ElementInfo> table);
		std::vector<PmergeMe::ElementInfo>	buildChain( const std::vector<ElementInfo> &largeElements );
		std::vector<int>	FordJohnsonSort( std::vector<int> &vec );
		std::vector<int>	pair( std::vector<int> &vec );
		std::vector<int>	generateJacobsthalNumbers(size_t n);
		// std::vector<int>	_vec;
		// std::deque<int>		_deq;
};


#endif // !PMERGEME_HPP


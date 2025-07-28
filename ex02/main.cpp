#include <cstdlib>
#include <iomanip>
#include <iostream>
#include "PmergeMe.hpp"
#include <limits>
#include <vector>
#include "Debug.hpp"

int main (int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: ./PmergeMe 2 4 3 7 9" << std::endl;;
		return (1);
	}
	int					input;
	long				linput;
	char				*endptr;
	double				time;
	size_t				size;
	std::vector<int>	vec;
	std::deque<int>		deq;
	for (int i = 1; argv[i] != NULL; i++)
	{
		linput = strtol(argv[i], &endptr, 10);
		if (*endptr == 0 && linput >= std::numeric_limits<int>::min()
			&& linput <= static_cast<float>(std::numeric_limits<int>::max())
			&& linput >= 0)
			input = static_cast<int>(linput);
		else
		{
			std::cout << "Usage: ./PemergeMe <sequence of positive int numbers>" << std::endl;
			std::cout << "e.g. ./PmergeMe 2 4 3 7 9" << std::endl;
			std::cout << "Remark: please don't mix quotes in there, also, program accepts only positive numbers" << std::endl;
			return (1);
		}
		vec.push_back(input);
		deq.push_back(input);
	}
	size = vec.size();
	std::cout << YEL << "Before: " << RESET;
	printContainer(vec);
	std::cout << YEL << "After:  " << RESET;
	printContainer(vec);
	time = stopwatch(PmergeMe::mergeInsertionSortVec, vec);
	std::cout << std::fixed << std::setprecision(5) << "Time to process a range of "
		<< size << " elements with std::vector<int> : "
		<< time * 1000.0 << " ms" << std::endl;
	std::cout << "size is same? ";
	if (vec.size() == size)
		std::cout << GREEN << "yes" << RESET << std::endl;
	else
		std::cout << RED << "no" << RESET << std::endl;
	std::cout << "is sorted: ";
	if (is_sorted(vec.begin(), vec.end()))
		std::cout << GREEN << "yes" << RESET << std::endl;
	else
		std::cout << RED << "no" << RESET << std::endl;
	std::cout << "comparison count: " << PmergeMe::comparisonCount << std::endl;
	return 0;
}

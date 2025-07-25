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
	double					time;
	std::vector<int>	vec;
	std::deque<int>		deq;
	for (int i = 1; argv[i] != NULL; i++)
	{
		linput = strtol(argv[i], &endptr, 10);
		if (*endptr == 0 && linput >= std::numeric_limits<int>::min()
			&& linput <= static_cast<float>(std::numeric_limits<int>::max()))
			input = static_cast<int>(linput);
		else
		{
			std::cout << "Usage: ./PmergeMe 2 4 3 7 9" << std::endl;
			std::cout << "Remark: please don't mix quotes in there" << std::endl;
			return (1);
		}
		vec.push_back(input);
		deq.push_back(input);
	}
	std::cout << YEL << "Before: " << RESET;
	printContainer(vec);
	// PmergeMe pm;
	// std::vector<int> inderstionOrder = pm.generateInsertionSequence(7);
	// printContainer(inderstionOrder);
	time = stopwatch(PmergeMe::mergeInsertionSortVec, vec);
	// pm.mergeInsertionSortVec(vec);
	std::cout << std::fixed << std::setprecision(2) << "Time taken: " << time << " seconds" << std::endl;
	std::cout << YEL << "After: " << RESET;
	printContainer(vec);
	std::cout << "is sorted: " << (is_sorted(vec.begin(), vec.end()) ? "yes" : "no") << std::endl;
	std::cout << "comparison count: " << PmergeMe::comparisonCount << std::endl;
	return 0;
}

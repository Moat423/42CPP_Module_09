#include <exception>
#include <iostream>
#include "RPN.hpp"

int main (int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <quoted expression in reverse polish notation>" << std::endl;
		return 1;
	}
	RPN rpn;
	try {
		rpn.calculate(std::string(argv[1]));
	} catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

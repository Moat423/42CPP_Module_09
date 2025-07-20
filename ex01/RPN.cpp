#include "RPN.hpp"
#include "Debug.hpp"
#include <iostream>

// Default Constructor
RPN::RPN( void )
{
	debug("RPN Default Constructor called");
}

// Parameterized Constructor
RPN::RPN( char * input)
{
	debug("RPN Parameterized Constructor called with value: " + input);
}

// Destructor
RPN::~RPN()
{
	debug("RPN Destructor called");
}

// Copy Constructor
RPN::RPN(const RPN &copy): _stack(copy._stack)
{
	debug("RPN Copy Constructor called");
	debug("Copyied stack size: " + std::to_string(_stack.size()));
}

// Copy Assignment Operator
RPN& RPN::operator=( const RPN &assign )
{
	if (this != &assign)
		this->_stack = assign._stack;
	return (*this);
}

void RPN::calculate( void )
{

}

void RPN::print( void ) const
{
	std::stack<int> tmp(_stack);
	while (!tmp.empty())
	{
		std::cout << tmp.top();
		tmp.pop();
		if (!tmp.empty())
			std::cout << " ";
	}
}

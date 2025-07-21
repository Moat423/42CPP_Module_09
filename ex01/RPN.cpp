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
	debug("RPN Parameterized Constructor called with value: ");
	debug(input);
	for (int i = 0; input[i] != '\0'; ++i)
	{
		if (isdigit(input[i]))
			_stack.push(input[i++] - '0');
		else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/')
			_stack.push(input[i++]);
		if (input[i] == ' ')
			continue;
		if (!input[i])
			return ;
		throw std::runtime_error("Invalid character in input: " + std::string(1, input[i]));
		return ;
	}
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
}

// Copy Assignment Operator
RPN& RPN::operator=( const RPN &assign )
{
	if (this != &assign)
		this->_stack = assign._stack;
	return (*this);
}

int RPN::operation( int left, int right, int current )
{
	switch (current) {
		case ('+'):
			return (left + right);
		case ('-'):
			return (left - right);
		case ('*'):
			return (left * right);
		case ('/'):
			return (left / right);
		default:
			throw invalidRPNExpressionException();
	}
}

void RPN::calculate( void )
{
	int	result;
	int	right;
	int	left;

	while (!_stack.empty())
	{
		int	current = _stack.top();
		if (current == '+' || current == '-' || current == '*' || current == '/')
		{
			_stack.pop();
			left = _stack.top();
			_stack.pop();
			right = _stack.top();
			operation(left, right, static_cast<char>(current));
		}
	}
}

void RPN::print( void ) const
{
	std::stack<int> tmp(_stack);
	while (tmp.size() > 1)
	{
		int	current = tmp.top();
		if (current >= 42 && current <= 47)
			std::cout << static_cast<char>(current);
		else
			std::cout << current;
		tmp.pop();
		std::cout << " ";
	}
	std::cout << tmp.top();
	tmp.pop();
	std::cout << std::endl;
}

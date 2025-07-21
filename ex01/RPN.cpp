#include "RPN.hpp"
#include "Debug.hpp"
#include <iostream>
#include <sstream>


// Default Constructor
RPN::RPN( void )
{
	debug("RPN Default Constructor called");
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
			throw std::runtime_error("invalid reverse polish notation expresion at: "
					+ std::string(1, static_cast<char>(left + '0'))
					+ std::string(1, static_cast<char>('0' + right))
					+ std::string(1, static_cast<char>(current)));
	}
}

void RPN::operationOnStack( char operand )
{
	int	right;
	int	left;
	int	result;

	if (_stack.size() < 2)
		throw InsufficientOperandsException();
	right = _stack.top();
	_stack.pop();
	left = _stack.top();
	_stack.pop();
	result = operation(left, right, operand);
	_stack.push(result);
}

void RPN::calculate( std::string input )
{
	std::string			token;
	std::istringstream	iss(input);

	while (iss >> token)
	{
		if (token.length() == 1 && isdigit(token[0]))
			_stack.push(token[0] - '0');
		else if (token.length() == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/'))
			operationOnStack(token[0]);
		else
			throw std::runtime_error("Invalid token in input: " + std::string(1, token[0]));
	}
	if (_stack.size() != 1)
	{
		print();
		throw ExcessOperandsException();
	}
	std::cout << _stack.top() << std::endl;
	return ;
}

void RPN::print( void ) const
{
	std::stack<int> tmp(_stack);
	while (tmp.size() > 0)
	{
		int	current = tmp.top();
		if (current >= 42 && current <= 47)
			std::cout << static_cast<char>(current);
		else
			std::cout << current;
		tmp.pop();
		if (tmp.size() > 1)
			std::cout << " ";
		else
			std::cout << std::endl;
	}
}

const char * RPN::InsufficientOperandsException::what() const throw()
{
	return ("Insufficient Operands for the Operation");
}

const char * RPN::ExcessOperandsException::what() const throw()
{
	return ("Excess Operands for the Operation");
}

const char * RPN::InvalidTokenException::what() const throw()
{
	return ("Invalid Token in Sequence");
}

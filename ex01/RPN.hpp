#ifndef RPN_HPP
# define RPN_HPP

#include <exception>
#include <stack>
#include <string>

class RPN {
	public:
		RPN( void );
		virtual ~RPN();

		RPN(const RPN &copy);
		RPN&	operator=( const RPN &assign );
		void	calculate( std::string input );
		void	print( void ) const;
		class InsufficientOperandsException: public std::exception
	  	{
			const char * what() const throw();
		};
		class ExcessOperandsException: public std::exception
	  	{
			const char * what() const throw();
		};
		class InvalidTokenException: public std::exception
	  	{
			const char * what() const throw();
		};
	private:
		int		operation( int left, int right, int current );
		void	operationOnStack( char operand );

		std::stack<int>	_stack;
};


#endif // !RPN_HPP


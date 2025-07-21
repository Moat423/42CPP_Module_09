#ifndef RPN_HPP
# define RPN_HPP

#include <exception>
#include <stack>
class RPN {
	public:
		RPN( char * input );
		virtual ~RPN();

		RPN(const RPN &copy);
		RPN& operator=( const RPN &assign );
		void calculate( void );
		void print( void ) const;
		class invalidRPNExpressionException: public std::exception
		{

		};

	private:
		RPN( void );
		int operation( int left, int right, int current );
		std::stack<int>	_stack;
};


#endif // !RPN_HPP


#ifndef RPN_HPP
# define RPN_HPP

#include <stack>
class RPN {
	public:
		RPN( char * input );
		virtual ~RPN();

		RPN(const RPN &copy);
		RPN& operator=( const RPN &assign );
		void calculate( void );
		void print( void ) const;

	private:
		RPN( void );
		std::stack<int>	_stack;
};


#endif // !RPN_HPP


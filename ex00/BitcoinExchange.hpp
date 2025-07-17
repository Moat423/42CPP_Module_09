#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <exception>
#include <map>
#include <string>

class Btc {
	public:
		Btc( void );
		virtual ~Btc();

		Btc(const Btc &copy);
		Btc& operator=( const Btc &assign );
		
		void	processInputFile(std::string filename);
		//exceptions
		class	notOpenException: public std::exception
		{
			public:
				virtual const char * what() const throw();
		};
	private:
		void	open_file();
		void	validate_file();
		std::map<int, std::string> _m;
};


#endif // !BITCOINEXCHANGE_HPP


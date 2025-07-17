#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <ctime>
#include <exception>
#include <map>
#include <string>

class Btc {
	public:
		Btc( void );
		Btc( std::string dataFilename );
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
		bool	parseDatabaseLine(std::string line);
		std::map<time_t, float> _m;
};


#endif // !BITCOINEXCHANGE_HPP


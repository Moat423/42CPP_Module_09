#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <ctime>
#include <exception>
#include <map>
#include <string>

class BitcoinExchange {
	public:
		BitcoinExchange( void );
		BitcoinExchange( std::string dataFilename );
		virtual ~BitcoinExchange();

		BitcoinExchange(const BitcoinExchange &copy);
		BitcoinExchange& operator=( const BitcoinExchange &assign );
		
		void	processInputFile(char *filename);
		//exceptions
		class	notOpenException: public std::exception
		{
			public:
				virtual const char * what() const throw();
		};
		void	printMapTimeFloat(void);
	private:
		void	open_file();
		void	validate_file();
		std::tm parseDate(std::string date);
		void	writeOutput(std::tm time, float value);
		bool	parseFileLine(std::string line);
		bool	parseDatabaseLine(std::string line);
		std::map<time_t, float> _m;
};


#endif // !BITCOINEXCHANGE_HPP


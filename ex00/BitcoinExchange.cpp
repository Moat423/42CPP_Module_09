#include "BitcoinExchange.hpp"
#include <ctime>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>

Btc::Btc(): _m()
{
}

void	Btc::processInputFile(std::string filename)
{
	std::ifstream fstream;
	fstream.open(filename, std::ios_base::in);
	if (!fstream.is_open())
		throw std::runtime_error("Error: could not open file");
}

int parseStringToInt(const std::string& s) {
    std::istringstream ss(s);
    int val;
    if (!(ss >> val))
        throw std::runtime_error("Error: invalid integer substring -> " + s);
    return val;
}

// works with any human normal times
bool	isValidDate(int	year, int month, int day)
{
	static const int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	bool	leapyear;
	if (year < 1900 || month < 1 || month > 12)
		return (false);
	int dim = daysInMonth[month - 1];
	if (month == 2)
	{
		leapyear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
		if (leapyear)
			dim++;
	}
	return (day > 0 && day <= dim);
}

// works with date in format: 2011-01-03
time_t	parseDatabaseDate(std::string date)
{
	std::tm	time = {};
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		throw std::runtime_error("Error: invalid Date in Database");
	int year, month, day;
	year = parseStringToInt(date.substr(0,4));
	month = parseStringToInt(date.substr(5,2));
	day = parseStringToInt(date.substr(8,2));
	if (!isValidDate(year, month, day))
		throw std::runtime_error("Error: invalid Date in Database");
	time.tm_year = year - 1900;
	time.tm_mon = month - 1;
	time.tm_mday = day;
	return (std::mktime(&time));
}

bool	Btc::parseDatabaseLine(std::string line)
{
	std::string	date;
	float		exchangeRate;
	time_t		key;
	std::stringstream ss(line);
	if (!std::getline(ss, date, ','))
		return (false);
	if (!(ss >> exchangeRate) || ss.fail())
		return (false);
	char extraGarbageAfterNumber;
	if (ss >> extraGarbageAfterNumber)
		return false;
	key = parseDatabaseDate(date);
	this->_m[key] = exchangeRate;
	return (true);
}

Btc::Btc(std::string dataFilename): _m()
{
	std::ifstream dataStream("data.csv");
	std::string line;
	std::string	date, value;

	if (!dataStream.is_open())
		throw std::runtime_error("Error: could not open databank file");
	std::getline(dataStream, line);
	while (std::getline(dataStream, line))
	{
		if (!parseDatabaseLine(line))
			throw std::runtime_error("Error: incorrectly formatted Databse");
	}

}


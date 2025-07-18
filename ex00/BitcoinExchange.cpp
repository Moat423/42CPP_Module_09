#include "BitcoinExchange.hpp"
#include <ctime>
#include <exception>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>

BitcoinExchange::BitcoinExchange(): _m()
{
}

BitcoinExchange::~BitcoinExchange()
{
	_m.clear();
}

//function used to test if data.csv was processed correctly
//redirect output to file and do diff
void	BitcoinExchange::printMapTimeFloat(void)
{
	for (std::map<time_t, float>::iterator it = _m.begin(); it != _m.end(); it++)
	{
		struct tm* timeInfo = localtime(&it->first);
		char timeString[11];
		strftime(timeString, sizeof(timeString), "%Y-%m-%d", timeInfo);
		std::cout << timeString << "," << it->second << std::endl;
	}
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
std::tm BitcoinExchange::parseDate(std::string date)
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
	return (time);
}

bool	BitcoinExchange::parseFileLine(std::string line)
{
	std::string	date;
	float		value;
	std::tm		tmpTime;
	time_t		key;
	std::stringstream ss(line);
	if (!std::getline(ss, date, ' '))
		return (false);
	if (!(ss >> value) || ss.fail())
		return (false);
	char extraGarbageAfterNumber;
	if (ss >> extraGarbageAfterNumber)
		return false;
	tmpTime = parseDate(date);
	key = std::mktime(&tmpTime);
	this->_m[key] = value;
	return (true);
}

void	printError(std::string errortype)
{
	std::cerr << "Error: " << errortype << std::endl;
}

// takes input time and input value and multiplies value by database exchange rate
void	BitcoinExchange::writeOutput(std::tm time, float value)
{
	char	timeString[11];
	float	result;
	time_t	inputTime = (std::mktime(&time));
	std::map<std::time_t, float>::iterator	it;
	strftime(timeString, sizeof(timeString), "%Y-%m-%d", &time);
	it = _m.upper_bound(inputTime);
	if (it == _m.begin())
	{
		std::cerr << "Error: no exchange rate available for " << timeString << std::endl;
		return ;
	}
	--it;
	result = value * (it->second);
	std::cout << timeString << " => " << value << " = " << result << std::endl;
}

void	BitcoinExchange::processInputFile(char *filename)
{
	std::ifstream	fstream;
	std::string		line;
	std::string		date;
	std::tm			time;
	std::string		seperator;
	float			value;

	if (_m.empty())
		throw std::runtime_error("No database provided.");
	fstream.open(filename, std::ios_base::in);
	if (!fstream.is_open())
		throw std::runtime_error("Error: could not open file.");
	std::getline(fstream, line);
	while (std::getline(fstream, line))
	{
		std::stringstream ss(line);
		if (!std::getline(ss, date, ' '))
			std::cerr << "Error: bad input => " << date << std::endl;
		try {
		time = parseDate(date);
		} catch (std::exception &e) {
			std::cerr << "Error: bad input => " << date << std::endl;
			continue ;
		}
		if (!std::getline(ss >> std::ws, seperator, ' ') && seperator != "|")
			std::cerr << "Error: bad input => " << line << std::endl;
		if (!(ss >> value))
			std::cerr << "Error: bad value in line => " << line << std::endl;
		if (value < 0)
		{
			std::cerr << "Error: not a positive number." << std::endl;
			continue ;
		}
		if (value > 1000)
		{
			std::cerr << "Error: too large a number." << std::endl;
			continue ;
		}
		writeOutput(time, value);
	}
}

bool	BitcoinExchange::parseDatabaseLine(std::string line)
{
	std::string	date;
	float		exchangeRate;
	std::tm		tmpTime;
	time_t		key;
	std::stringstream ss(line);
	if (!std::getline(ss, date, ','))
		return (false);
	if (!(ss >> exchangeRate) || ss.fail())
		return (false);
	char extraGarbageAfterNumber;
	if (ss >> extraGarbageAfterNumber)
		return false;
	tmpTime = parseDate(date);
	key = std::mktime(&tmpTime);
	this->_m[key] = exchangeRate;
	return (true);
}

BitcoinExchange::BitcoinExchange(std::string dataFilename): _m()
{
	std::ifstream dataStream(dataFilename.c_str());
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

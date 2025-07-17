#include "BitcoinExchange.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

Btc::Btc()
{
	std::ifstream dataStream("data.csv");
	std::string line;

	if (!dataStream.is_open())
		throw std::runtime_error("Error: could not open databank file");
	std::getline(dataStream, line);
	while (std::getline(dataStream, line))
	{

	}

}


void	Btc::processInputFile(std::string filename)
{
	std::ifstream fstream;
	fstream.open(filename, std::ios_base::in);
	if (!fstream.is_open())
		throw std::runtime_error("Error: could not open file");
}

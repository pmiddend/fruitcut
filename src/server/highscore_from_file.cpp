#include "highscore_from_file.hpp"
#include <string>
#include <stdexcept>
#include <fstream>

fruitcut::server::highscore_sequence const 
fruitcut::server::highscore_from_file(
	std::string const &s)
{
	std::ifstream stream(
		s.c_str());

	fruitcut::server::highscore_sequence result;

	if(!stream.is_open())
		return result;

	char const separator = 
		0;

	std::string name,score,datetime;
	while(
		std::getline(
			stream,
			name,
			separator) &&
		std::getline(
			stream,
			score,
			separator) &&
		std::getline(
			stream,
			datetime,
			separator))
	{
		result.push_back(
			server::highscore_entry(
				name,
				score,
				datetime));
	}

	return result;
}

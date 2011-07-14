#include "highscore_to_string.hpp"

std::string
fruitcut::server::highscore_to_string(
	server::highscore_sequence const &h)
{
	std::string result;

	for(
		server::highscore_sequence::const_iterator it = 
			h.begin();
		it != h.end();
		++it)
	{
		result += it->name();
		result.append(
			static_cast<std::string::size_type>(
				1),
			static_cast<char>(
				0));
		result += it->score();
		result.append(
			static_cast<std::string::size_type>(
				1),
			static_cast<char>(
				0));
		result += it->datetime();
		result.append(
			static_cast<std::string::size_type>(
				1),
			static_cast<char>(
				0));
	}

	return result;
}

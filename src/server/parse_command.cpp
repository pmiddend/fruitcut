#include "parse_command.hpp"
#include <string>

fruitcut::server::command const
fruitcut::server::parse_command(
	std::string const &s)
{
	server::command parts;

	std::string::size_type previous = 0;
	for(
		std::string::size_type i = 0;
		i < s.size();
		++i)
	{
		if(s[i] == static_cast<char>(10))
			continue;

		parts.push_back(
			s.substr(
				previous,
				static_cast<std::string::size_type>(
					i - previous)));

		previous = 
			static_cast<std::string::size_type>(
				i+1);
	}

	parts.push_back(
		s.substr(
			previous,
			static_cast<std::string::size_type>(
				s.size() - previous)));

	return 
		parts;
}

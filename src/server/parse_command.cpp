#include "parse_command.hpp"
#include "ascii/from_native_char.hpp"

fruitcut::server::command const
fruitcut::server::parse_command(
	ascii::string const &s)
{
	server::command parts;

	ascii::string::size_type previous = 0;
	for(
		ascii::string::size_type i = 0;
		i < s.size();
		++i)
	{
		if(s[i].value() != ascii::from_native_char(' ').value())
			continue;

		parts.push_back(
			s.substr(
				previous,
				static_cast<ascii::string::size_type>(
					i - previous)));

		previous = 
			static_cast<ascii::string::size_type>(
				i+1);
	}

	parts.push_back(
		s.substr(
			previous,
			static_cast<ascii::string::size_type>(
				s.size() - previous)));

	return 
		parts;
}

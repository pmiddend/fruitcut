#include <fruitserver/parse_command.hpp>
#include <fruitserver/ascii/from_native_char.hpp>

fruitserver::command const
fruitserver::parse_command(
	ascii::string const &s)
{
	fruitserver::command parts;

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

#include "from_native.hpp"
#include "from_native_char.hpp"

fruitcut::server::ascii::string const
fruitcut::server::ascii::from_native(
	std::string const &s)
{
	ascii::string result;
	result.resize(
		s.size());
	for(std::string::size_type i = 0; i < s.size(); ++i)
		result[i].value(
			ascii::from_native_char(
				s[i]).value());
	return result;
}
#include "to_native.hpp"
#include "to_native_char.hpp"

std::string const
fruitcut::server::ascii::to_native(
	ascii::string const &s)
{
	std::string result;
	result.resize(
		s.size());
	for(
		std::string::size_type i = 
			0; 
		i < s.size(); 
		++i)
		result[i] = 
			ascii::to_native_char(
				s[i]);
	return result;
}

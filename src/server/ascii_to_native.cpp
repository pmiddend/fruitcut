#include "ascii_to_native.hpp"
#include "ascii_to_native_char.hpp"

std::string
fruitcut::server::ascii_to_native(
	std::string const &s)
{
	std::string result;
	result.resize(
		s.size());
	for(std::string::size_type i = 0; i < s.size(); ++i)
		result[i] = 
			server::ascii_to_native_char(
				s[i]);
	return result;
}

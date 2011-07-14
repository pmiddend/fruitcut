#include "native_to_ascii.hpp"
#include "native_to_ascii_char.hpp"

std::string
fruitcut::server::native_to_ascii(
	std::string const &s)
{
	std::string result;
	result.resize(
		s.size());
	for(std::string::size_type i = 0; i < s.size(); ++i)
		result[i] = 
			server::native_to_ascii_char(
				s[i]);
	return result;
}

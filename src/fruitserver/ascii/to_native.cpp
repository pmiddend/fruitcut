#include <fcppt/config/external_begin.hpp>
#include <fruitserver/ascii/to_native.hpp>
#include <fruitserver/ascii/to_native_char.hpp>
#include <fcppt/config/external_end.hpp>


std::string const
fruitserver::ascii::to_native(
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

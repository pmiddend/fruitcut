#include <fcppt/config/external_begin.hpp>
#include <fruitserver/ascii/from_native.hpp>
#include <fruitserver/ascii/from_native_char.hpp>
#include <fcppt/config/external_end.hpp>


fruitserver::ascii::string const
fruitserver::ascii::from_native(
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

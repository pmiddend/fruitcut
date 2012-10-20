#include <fruitserver/ascii/to_byte_sequence.hpp>


fruitserver::byte_sequence const
fruitserver::ascii::to_byte_sequence(
	ascii::string const &s)
{
	fruitserver::byte_sequence result(
		static_cast<fruitserver::byte_sequence::size_type>(
			s.size()));
	for(
		fruitserver::byte_sequence::size_type i = 0;
		i < result.size();
		++i)
		result[i] =
			s[static_cast<ascii::string::size_type>(i)].value();
	return result;
}

#include <fruitserver/ascii/from_byte_sequence.hpp>

fruitserver::ascii::string const
fruitserver::ascii::from_byte_sequence(
	fruitserver::byte_sequence const &bytes)
{
	ascii::string s;
	s.resize(
		static_cast<ascii::string::size_type>(
			bytes.size()));
	for(
		ascii::string::size_type i = 
			0;
		i != s.size();
		++i)
		s[i].value(
			bytes[static_cast<fruitserver::byte_sequence::size_type>(i)]);
	return s;
}

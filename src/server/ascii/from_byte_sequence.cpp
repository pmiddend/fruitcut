#include "from_byte_sequence.hpp"

fruitcut::server::ascii::string const
fruitcut::server::ascii::from_byte_sequence(
	server::byte_sequence const &bytes)
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
			bytes[static_cast<server::byte_sequence::size_type>(i)]);
	return s;
}

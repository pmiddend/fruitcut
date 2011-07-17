#include "to_byte_sequence.hpp"

fruitcut::server::byte_sequence const
fruitcut::server::ascii::to_byte_sequence(
	ascii::string const &s)
{
	server::byte_sequence result(
		static_cast<server::byte_sequence::size_type>(
			s.size()));
	for(
		server::byte_sequence::size_type i = 0;
		i < result.size();
		++i)
		result[i] = 
			s[static_cast<ascii::string::size_type>(i)].value();
	return result;
}

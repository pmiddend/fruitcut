#include "ascii_to_native_char.hpp"
#include "ascii_native_translation.hpp"
#include <cstddef>

namespace
{
char
ascii_to_native_char_impl(
	std::size_t const c)
{
	return 
		c > 127u
		?
			static_cast<char>(
				0)
		:
			fruitcut::server::ascii_native_translation()[
				c];
}
}

char
fruitcut::server::ascii_to_native_char(
	char const c)
{
	return 
		ascii_to_native_char_impl(
			static_cast<std::size_t>(
				c));
}

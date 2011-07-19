#include "to_native_char.hpp"
#include "to_native_translation.hpp"

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
			fruitcut::server::ascii::to_native_translation()[
				c];
}
}

char
fruitcut::server::ascii::to_native_char(
	ascii::char_ const &c)
{
	return 
		ascii_to_native_char_impl(
			static_cast<std::size_t>(
				c.value()));
}
#include "is_digit.hpp"

bool
fruitcut::server::ascii::is_digit(
	ascii::char_ const &c)
{
	return c.value() >= 48 && c.value() <= 57;
}

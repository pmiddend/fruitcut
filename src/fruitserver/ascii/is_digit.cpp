#include "is_digit.hpp"

bool
fruitserver::ascii::is_digit(
	ascii::char_ const &c)
{
	return c.value() >= 48 && c.value() <= 57;
}

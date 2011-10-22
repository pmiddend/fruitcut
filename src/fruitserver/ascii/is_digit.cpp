#include <fcppt/config/external_begin.hpp>
#include <fruitserver/ascii/is_digit.hpp>
#include <fcppt/config/external_end.hpp>


bool
fruitserver::ascii::is_digit(
	ascii::char_ const &c)
{
	return c.value() >= 48 && c.value() <= 57;
}

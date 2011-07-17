#include "is_letter.hpp"

bool
fruitcut::server::ascii::is_letter(
	ascii::char_ const &c)
{
	return 
		(c.value() >= 65 && c.value() <= 90) || 
		(c.value() >= 97 && c.value() <= 122);
}

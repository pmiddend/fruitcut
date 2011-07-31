#include "separator.hpp"
#include "ascii/from_native_char.hpp"

fruitserver::ascii::char_ const
fruitserver::separator()
{
	return 
		ascii::from_native_char(
			'"');
}

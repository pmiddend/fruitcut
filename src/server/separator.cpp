#include "separator.hpp"
#include "ascii/from_native_char.hpp"

fruitcut::server::ascii::char_ const
fruitcut::server::separator()
{
	return 
		ascii::from_native_char(
			'"');
}

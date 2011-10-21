#include <fcppt/config/external_begin.hpp>
#include <fruitserver/separator.hpp>
#include <fruitserver/ascii/from_native_char.hpp>
#include <fcppt/config/external_end.hpp>


fruitserver::ascii::char_ const
fruitserver::separator()
{
	return
		ascii::from_native_char(
			'"');
}

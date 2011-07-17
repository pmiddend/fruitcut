#include "from_native_char.hpp"
#include "to_native_translation.hpp"
#include <stdexcept>
#include <string>

fruitcut::server::ascii::char_ const
fruitcut::server::ascii::from_native_char(
	char const c)
{
	std::string const translation = 
		ascii::to_native_translation();

	for(std::string::size_type i = 0; i < translation.size(); ++i)
		if(translation[i] == c)
			return 
				ascii::char_(
					static_cast<ascii::char_::value_type>(
						i));

	throw 
		std::runtime_error(
			std::string("Found native char that cannot be translated: ")+c);
}

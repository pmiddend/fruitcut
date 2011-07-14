#include "native_to_ascii_char.hpp"
#include "ascii_native_translation.hpp"
#include <string>
#include <stdexcept>

char
fruitcut::server::native_to_ascii_char(
	char const c)
{
	std::string const translation = 
		server::ascii_native_translation();

	for(std::string::size_type i = 0; i < translation.size(); ++i)
		if(translation[i] == c)
			return 
				static_cast<char>(
					i);

	throw std::runtime_error(std::string("Found native char that cannot be translated: ")+c);
}

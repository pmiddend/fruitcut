#include "ascii_to_native_char.hpp"
#include <cstddef>

namespace
{
char const translation_matrix[] = 
	{
		// control character stuff
		0,0,0,0,0,0,0,0,'\v','\n','\t',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/',
		'0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?','@',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'[','\\',']','^','_','`',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		'{','|','}','~',0
	};

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
			translation_matrix[
				c];
}
}

char
fruitcut::server::ascii_to_native_char(
	char const c)
{
	return 
		ascii_to_native_char_impl(
			static_cast<std::size_t>(
				c));
}

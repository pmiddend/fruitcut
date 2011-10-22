#include <fcppt/config/external_begin.hpp>
#include <fruitserver/ascii/to_native_translation.hpp>
#include <fcppt/config/external_end.hpp>


namespace
{
char const translation_matrix[] =
	{
		// control character stuff
		0, // null
		0, // start of heading
		0, // start of text
		0, // end of text
		0, // end of transmission
		0, // enquiry
		0, // acknowledge
		0, // bell
		0, // backspace
		'\t', // horizontal tab
		'\n', // newline
		'\v', // vertical tab
		0, // new page
		0, // carriage return
		0, // shift out
		0, // shift in
		0, // data link escape
		0, // device control 1
		0, // device control 2
		0, // device control 3
		0, // device control 4
		0, // negative acknowledge
		0, // synchronous idle
		0, // end of transmission block
		0, // cancel
		0, // end of medium
		0, // substitute
		0, // escape
		0, // file separator
		0, // group separator
		0, // record separator
		0, // unit separator
		' ',
		'!',
		'"',
		'#',
		'$',
		'%',
		'&',
		'\'',
		'(',
		')',
		'*',
		'+',
		',',
		'-',
		'.',
		'/',
		'0','1','2','3','4','5','6','7','8','9',
		':',
		';',
		'<',
		'=',
		'>',
		'?',
		'@',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'[',
		'\\',
		']',
		'^',
		'_',
		'`',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		'{',
		'|',
		'}',
		'~',
		0 // DEL (whatever that is)
	};
}

std::string
fruitserver::ascii::to_native_translation()
{
	return
		std::string(
			&translation_matrix[0],
			sizeof(
				translation_matrix));
}

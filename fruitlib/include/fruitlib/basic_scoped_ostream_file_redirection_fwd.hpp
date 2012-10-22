#ifndef FRUITLIB_BASIC_SCOPED_OSTREAM_FILE_REDIRECTION_FWD_HPP_INCLUDED
#define FRUITLIB_BASIC_SCOPED_OSTREAM_FILE_REDIRECTION_FWD_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <string>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
template
<
	typename Char,
	typename CharTraits = std::char_traits<Char>
>
class basic_scoped_ostream_file_redirection;
}

#endif


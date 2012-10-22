#ifndef FRUITLIB_SCOPED_WOSTREAM_FILE_REDIRECTION_HPP_INCLUDED
#define FRUITLIB_SCOPED_WOSTREAM_FILE_REDIRECTION_HPP_INCLUDED

#include <fruitlib/basic_scoped_ostream_file_redirection_decl.hpp>

namespace fruitlib
{
typedef
fruitlib::basic_scoped_ostream_file_redirection<wchar_t>
scoped_wostream_file_redirection;
}

#endif


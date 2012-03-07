#ifndef FRUITLIB_UTF8_FILE_TO_FCPPT_STRING_HPP_INCLUDED
#define FRUITLIB_UTF8_FILE_TO_FCPPT_STRING_HPP_INCLUDED

#include <sge/charconv/system_fwd.hpp>
#include <fcppt/string.hpp>
#include <fcppt/filesystem/path.hpp>

namespace fruitlib
{
fcppt::string const
utf8_file_to_fcppt_string(
	sge::charconv::system &,
	fcppt::filesystem::path const &);
}

#endif

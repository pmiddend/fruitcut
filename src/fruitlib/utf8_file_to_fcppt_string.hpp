#ifndef FRUITCUT_FRUITLIB_UTF8_FILE_TO_FCPPT_STRING_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_UTF8_FILE_TO_FCPPT_STRING_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/filesystem/path.hpp>

namespace fruitcut
{
namespace fruitlib
{
fcppt::string const
utf8_file_to_fcppt_string(
	fcppt::filesystem::path const &);
}
}

#endif

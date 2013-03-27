#ifndef FRUITLIB_UTF8_FILE_TO_FCPPT_STRING_EXN_HPP_INCLUDED
#define FRUITLIB_UTF8_FILE_TO_FCPPT_STRING_EXN_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <fcppt/config/external_end.hpp>

namespace fruitlib
{
fcppt::string const
utf8_file_to_fcppt_string_exn(
	boost::filesystem::path const &);
}

#endif


#ifndef FRUITLIB_FCPPT_STRING_TO_UTF8_FILE_HPP_INCLUDED
#define FRUITLIB_FCPPT_STRING_TO_UTF8_FILE_HPP_INCLUDED

#include <sge/charconv/system_fwd.hpp>
#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
bool
fcppt_string_to_utf8_file(
	fcppt::string const &,
	boost::filesystem::path const &,
	sge::charconv::system &);
}

#endif


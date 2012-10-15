#ifndef FRUITLIB_FONT_BASE_PATH_HPP_INCLUDED
#define FRUITLIB_FONT_BASE_PATH_HPP_INCLUDED

#include <fcppt/strong_typedef.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <fcppt/config/external_end.hpp>

namespace fruitlib
{
namespace font
{
FCPPT_MAKE_STRONG_TYPEDEF(
	boost::filesystem::path,
	base_path);
}
}

#endif

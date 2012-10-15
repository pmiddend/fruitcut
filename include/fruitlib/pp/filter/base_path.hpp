#ifndef FRUITLIB_PP_FILTER_BASE_PATH_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_BASE_PATH_HPP_INCLUDED

#include <fcppt/strong_typedef.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <fcppt/config/external_end.hpp>

namespace fruitlib
{
namespace pp
{
namespace filter
{
FCPPT_MAKE_STRONG_TYPEDEF(
	boost::filesystem::path,
	base_path);
}
}
}

#endif

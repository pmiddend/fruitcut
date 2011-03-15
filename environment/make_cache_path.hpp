#ifndef FRUITCUT_ENVIRONMENT_MAKE_CACHE_PATH_HPP_INCLUDED
#define FRUITCUT_ENVIRONMENT_MAKE_CACHE_PATH_HPP_INCLUDED

#include <fcppt/filesystem/path.hpp>
#include <fcppt/string.hpp>

namespace fruitcut
{
namespace environment
{
fcppt::filesystem::path const
make_cache_path(
	fcppt::string const &app_name,
	fcppt::string const &desired_filename);
}
}

#endif

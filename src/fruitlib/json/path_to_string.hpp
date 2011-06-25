#ifndef FRUITCUT_FRUITLIB_JSON_PATH_TO_STRING_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_JSON_PATH_TO_STRING_HPP_INCLUDED

#include "path_fwd.hpp"
#include <fcppt/string.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace json
{
fcppt::string const
path_to_string(
	json::path const &);
}
}
}

#endif
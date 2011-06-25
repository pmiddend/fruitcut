#ifndef FRUITCUT_FRUITLIB_JSON_MODIFY_USER_VALUE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_JSON_MODIFY_USER_VALUE_HPP_INCLUDED

#include "path_fwd.hpp"
#include <sge/parse/json/json.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace json
{
/*
	This is a more specific function. It takes as arguments:

	- a "structure" json. This json object specifies how the "complete"
    configuration tree looks. Anything that's not in the structure
    tree doesn't exist and is considered invalid.
	- a "user" json. This stores a subset of the structure json and
    contains the configuration options the user specified him/herself.
	- a path to a specific option
	- the option's new value
 */
void
modify_user_value(
	sge::parse::json::object const &structure_json,
	sge::parse::json::object &user_json,
	json::path const &path,
	sge::parse::json::value const &new_value);
}
}
}

#endif

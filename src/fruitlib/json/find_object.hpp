#ifndef FRUITCUT_FRUITLIB_JSON_FIND_OBJECT_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_JSON_FIND_OBJECT_HPP_INCLUDED

#include <sge/parse/json/object_fwd.hpp>
#include "path_fwd.hpp"

namespace fruitcut
{
namespace fruitlib
{
namespace json
{
sge::parse::json::object *
find_object(
	sge::parse::json::object &,
	json::path const &);

sge::parse::json::object const *
find_object(
	sge::parse::json::object const &,
	json::path const &);
}
}
}

#endif

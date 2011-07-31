#ifndef FRUITLIB_JSON_MAKE_RECURSIVE_OBJECTS_HPP_INCLUDED
#define FRUITLIB_JSON_MAKE_RECURSIVE_OBJECTS_HPP_INCLUDED

#include <fruitlib/json/path_fwd.hpp>
#include <sge/parse/json/object.hpp>

namespace fruitlib
{
namespace json
{
sge::parse::json::object &
make_recursive_objects(
	sge::parse::json::object &,
	json::path const &);
}
}

#endif

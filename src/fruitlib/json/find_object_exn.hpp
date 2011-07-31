#ifndef FRUITLIB_JSON_FIND_OBJECT_EXN_HPP_INCLUDED
#define FRUITLIB_JSON_FIND_OBJECT_EXN_HPP_INCLUDED

#include "path_fwd.hpp"
#include <sge/parse/json/object_fwd.hpp>

namespace fruitlib
{
namespace json
{
sge::parse::json::object const &
find_object_exn(
	sge::parse::json::object const &,
	json::path const &);

sge::parse::json::object &
find_object_exn(
	sge::parse::json::object &,
	json::path const &);
}
}

#endif

#ifndef FRUITCUT_JSON_MERGE_TREES_HPP_INCLUDED
#define FRUITCUT_JSON_MERGE_TREES_HPP_INCLUDED

#include <sge/parse/json/object.hpp>

namespace fruitcut
{
namespace json
{
sge::parse::json::object const
merge_trees(
	sge::parse::json::object const &original,
	sge::parse::json::object const &update);
}
}

#endif

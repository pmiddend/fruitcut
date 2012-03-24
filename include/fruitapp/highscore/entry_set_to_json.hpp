#ifndef FRUITAPP_HIGHSCORE_ENTRY_SET_TO_JSON_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_ENTRY_SET_TO_JSON_HPP_INCLUDED

#include <fruitapp/highscore/entry_set.hpp>
#include <sge/parse/json/object_fwd.hpp>

namespace fruitapp
{
namespace highscore
{
sge::parse::json::object const
entry_set_to_json(
	highscore::entry_set const &);
}
}

#endif


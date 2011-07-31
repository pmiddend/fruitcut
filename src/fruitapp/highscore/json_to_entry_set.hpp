#ifndef FRUITAPP_HIGHSCORE_JSON_TO_ENTRY_SET_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_JSON_TO_ENTRY_SET_HPP_INCLUDED

#include "entry_set.hpp"
#include <sge/parse/json/object_fwd.hpp>

namespace fruitapp
{
namespace highscore
{
highscore::entry_set const
json_to_entry_set(
	sge::parse::json::object const &);
}
}

#endif

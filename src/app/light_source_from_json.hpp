#ifndef FRUITCUT_APP_LIGHT_SOURCE_FROM_JSON_HPP_INCLUDED
#define FRUITCUT_APP_LIGHT_SOURCE_FROM_JSON_HPP_INCLUDED

#include "directional_light_source.hpp"
#include <sge/parse/json/object_fwd.hpp>

namespace fruitcut
{
namespace app
{
directional_light_source const
light_source_from_json(
	sge::parse::json::object const &);
}
}

#endif

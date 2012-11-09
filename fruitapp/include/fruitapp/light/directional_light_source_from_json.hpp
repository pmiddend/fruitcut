#ifndef FRUITAPP_LIGHT_DIRECTIONAL_LIGHT_SOURCE_FROM_JSON_HPP_INCLUDED
#define FRUITAPP_LIGHT_DIRECTIONAL_LIGHT_SOURCE_FROM_JSON_HPP_INCLUDED

#include <fruitapp/light/directional_light_source.hpp>
#include <sge/parse/json/object_fwd.hpp>


namespace fruitapp
{
namespace light
{
fruitapp::light::directional_light_source const
directional_light_source_from_json(
	sge::parse::json::object const &);
}
}

#endif

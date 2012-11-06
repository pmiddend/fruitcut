#ifndef FRUITAPP_LIGHT_SOURCE_FROM_JSON_HPP_INCLUDED
#define FRUITAPP_LIGHT_SOURCE_FROM_JSON_HPP_INCLUDED

#include <fruitapp/directional_light_source.hpp>
#include <sge/parse/json/object_fwd.hpp>


namespace fruitapp
{
fruitapp::directional_light_source const
directional_light_source_from_json(
	sge::parse::json::object const &);
}

#endif

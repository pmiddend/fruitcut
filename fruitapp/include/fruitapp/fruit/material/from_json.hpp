#ifndef FRUITAPP_FRUIT_MATERIAL_FROM_JSON_HPP_INCLUDED
#define FRUITAPP_FRUIT_MATERIAL_FROM_JSON_HPP_INCLUDED

#include <fruitapp/fruit/material/object.hpp>
#include <sge/parse/json/object_fwd.hpp>


namespace fruitapp
{
namespace fruit
{
namespace material
{
material::object const
from_json(
	sge::parse::json::object const &);
}
}
}

#endif

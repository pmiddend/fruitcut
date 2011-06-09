#ifndef FRUITCUT_APP_FRUIT_MATERIAL_FROM_JSON_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MATERIAL_FROM_JSON_HPP_INCLUDED

#include "object.hpp"
#include <sge/parse/json/object_fwd.hpp>

namespace fruitcut
{
namespace app
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
}

#endif

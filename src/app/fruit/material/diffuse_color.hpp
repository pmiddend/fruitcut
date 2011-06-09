#ifndef FRUITCUT_APP_FRUIT_MATERIAL_DIFFUSE_COLOR_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MATERIAL_DIFFUSE_COLOR_HPP_INCLUDED

#include <sge/renderer/vector4.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/strong_typedef.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
namespace material
{
FCPPT_MAKE_STRONG_TYPEDEF(
	sge::renderer::vector4,
	diffuse_color);
}
}
}
}

#endif

#ifndef FRUITAPP_FRUIT_MATERIAL_SPECULAR_COLOR_HPP_INCLUDED
#define FRUITAPP_FRUIT_MATERIAL_SPECULAR_COLOR_HPP_INCLUDED

#include <sge/renderer/vector4.hpp>
#include <fcppt/strong_typedef.hpp>
#include <fcppt/math/vector/basic_impl.hpp>


namespace fruitapp
{
namespace fruit
{
namespace material
{
FCPPT_MAKE_STRONG_TYPEDEF(
	sge::renderer::vector4,
	specular_color);
}
}
}

#endif

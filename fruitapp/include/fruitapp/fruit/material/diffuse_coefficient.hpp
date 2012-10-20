#ifndef FRUITAPP_FRUIT_MATERIAL_DIFFUSE_COEFFICIENT_HPP_INCLUDED
#define FRUITAPP_FRUIT_MATERIAL_DIFFUSE_COEFFICIENT_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
#include <fcppt/strong_typedef.hpp>

namespace fruitapp
{
namespace fruit
{
namespace material
{
FCPPT_MAKE_STRONG_TYPEDEF(
	sge::renderer::scalar,
	diffuse_coefficient);
}
}
}

#endif

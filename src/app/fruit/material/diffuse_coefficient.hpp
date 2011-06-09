#ifndef FRUITCUT_APP_FRUIT_MATERIAL_DIFFUSE_COEFFICIENT_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MATERIAL_DIFFUSE_COEFFICIENT_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
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
	sge::renderer::scalar,
	diffuse_coefficient);
}
}
}
}

#endif

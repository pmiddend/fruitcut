#ifndef FRUITAPP_LIGHT_AMBIENT_INTENSITY_HPP_INCLUDED
#define FRUITAPP_LIGHT_AMBIENT_INTENSITY_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
#include <fcppt/strong_typedef.hpp>

namespace fruitapp
{
namespace light
{
FCPPT_MAKE_STRONG_TYPEDEF(
	sge::renderer::scalar,
	ambient_intensity);
}
}

#endif

#ifndef FRUITAPP_SHADOW_MAP_MVP_HPP_INCLUDED
#define FRUITAPP_SHADOW_MAP_MVP_HPP_INCLUDED

#include <sge/renderer/matrix4.hpp>
#include <fcppt/strong_typedef.hpp>


namespace fruitapp
{
namespace shadow_map
{
FCPPT_MAKE_STRONG_TYPEDEF(
	sge::renderer::matrix4,
	mvp);
}
}

#endif

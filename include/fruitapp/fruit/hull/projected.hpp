#ifndef FRUITAPP_FRUIT_HULL_PROJECTED_HPP_INCLUDED
#define FRUITAPP_FRUIT_HULL_PROJECTED_HPP_INCLUDED

#include <fruitapp/fruit/hull/ring.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/target_base_fwd.hpp>

namespace fruitapp
{
namespace fruit
{
namespace hull
{
ring const
projected(
	object const &,
	sge::renderer::target_base &,
	sge::renderer::matrix4 const &);
}
}
}

#endif

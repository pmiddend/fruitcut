#ifndef FRUITAPP_FRUIT_HULL_PROJECTED_HPP_INCLUDED
#define FRUITAPP_FRUIT_HULL_PROJECTED_HPP_INCLUDED

#include <fruitapp/fruit/object_fwd.hpp>
#include <fruitapp/fruit/hull/ring.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/target/base_fwd.hpp>


namespace fruitapp
{
namespace fruit
{
namespace hull
{
fruitapp::fruit::hull::ring const
projected(
	fruitapp::fruit::object const &,
	sge::renderer::target::base &,
	sge::renderer::matrix4 const &);
}
}
}

#endif

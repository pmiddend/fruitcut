#ifndef FRUITAPP_FRUIT_MESH_TO_SHAPE_HPP_INCLUDED
#define FRUITAPP_FRUIT_MESH_TO_SHAPE_HPP_INCLUDED

#include <fruitapp/fruit/mesh_fwd.hpp>
#include <fruitlib/physics/shared_shape_ptr.hpp>


namespace fruitapp
{
namespace fruit
{
fruitlib::physics::shared_shape_ptr const
mesh_to_shape(
	mesh const &);
}
}

#endif

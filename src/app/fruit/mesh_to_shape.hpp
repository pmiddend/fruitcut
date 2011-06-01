#ifndef FRUITCUT_APP_FRUIT_MESH_TO_SHAPE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MESH_TO_SHAPE_HPP_INCLUDED

#include "../../fruitlib/physics/shared_shape_ptr.hpp"
#include "mesh_fwd.hpp"

namespace fruitcut
{
namespace app
{
namespace fruit
{
fruitlib::physics::shared_shape_ptr const
mesh_to_shape(
	mesh const &);
}
}
}

#endif

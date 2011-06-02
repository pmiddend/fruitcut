#ifndef FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_CALLBACK_FN_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_CALLBACK_FN_HPP_INCLUDED

#include "collision_data.hpp"

namespace fruitcut
{
namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
typedef
void 
collision_callback_fn(
	rigid_body::collision_data const &);
}
}
}
}

#endif

#ifndef FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_CALLBACK_FN_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_CALLBACK_FN_HPP_INCLUDED

#include "collision_data.hpp"

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

#endif

#ifndef FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_CALLBACK_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_CALLBACK_HPP_INCLUDED

#include <fruitlib/physics/rigid_body/collision_callback_fn.hpp>
#include <fcppt/function/object.hpp>


namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
typedef
fcppt::function::object<collision_callback_fn>
collision_callback;
}
}
}

#endif

#ifndef FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_CALLBACK_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_CALLBACK_HPP_INCLUDED

#include "collision_callback_fn.hpp"
#include <fcppt/function/object.hpp>

namespace fruitcut
{
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
}

#endif

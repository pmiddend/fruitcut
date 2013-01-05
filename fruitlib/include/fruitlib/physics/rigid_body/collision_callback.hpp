#ifndef FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_CALLBACK_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_CALLBACK_HPP_INCLUDED

#include <fruitlib/physics/rigid_body/collision_callback_fn.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
typedef
std::function<collision_callback_fn>
collision_callback;
}
}
}

#endif

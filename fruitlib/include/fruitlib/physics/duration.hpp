#ifndef FRUITLIB_PHYSICS_DURATION_HPP_INCLUDED
#define FRUITLIB_PHYSICS_DURATION_HPP_INCLUDED

#include <fruitlib/physics/scalar.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
#include <fcppt/config/external_end.hpp>

namespace fruitlib
{
namespace physics
{
typedef
std::chrono::duration<fruitlib::physics::scalar>
duration;
}
}

#endif

#ifndef FRUITLIB_PHYSICS_DURATION_HPP_INCLUDED
#define FRUITLIB_PHYSICS_DURATION_HPP_INCLUDED

#include <fruitlib/physics/scalar.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <fcppt/config/external_end.hpp>

namespace fruitlib
{
namespace physics
{
typedef
boost::chrono::duration<fruitlib::physics::scalar>
duration;
}
}

#endif

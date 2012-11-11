#ifndef FRUITAPP_FRUIT_CALCULATE_NEW_LINEAR_VELOCITY_HPP_INCLUDED
#define FRUITAPP_FRUIT_CALCULATE_NEW_LINEAR_VELOCITY_HPP_INCLUDED

#include <fruitlib/physics/rigid_body/linear_velocity.hpp>
#include <fcppt/preprocessor/pure.hpp>

namespace fruitapp
{
namespace fruit
{
fruitlib::physics::rigid_body::linear_velocity const
calculate_new_linear_velocity(
	fruitlib::physics::rigid_body::linear_velocity const &old_velocity,
	fruitlib::physics::rigid_body::linear_velocity const &normal_velocity)
FCPPT_PP_PURE;
}
}

#endif

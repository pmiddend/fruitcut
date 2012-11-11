#include <fruitapp/fruit/calculate_new_linear_velocity.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>


fruitlib::physics::rigid_body::linear_velocity const
fruitapp::fruit::calculate_new_linear_velocity(
	fruitlib::physics::rigid_body::linear_velocity const &old_velocity,
	fruitlib::physics::rigid_body::linear_velocity const &normal_velocity)
{
	fruitlib::physics::scalar const old_to_new_velocity_factor =
		static_cast<fruitlib::physics::scalar>(
			0.5);

	return
		fruitlib::physics::rigid_body::linear_velocity(
			old_to_new_velocity_factor * old_velocity.get() +
			(1 - old_to_new_velocity_factor) *
			fcppt::math::vector::length(
				old_velocity.get()) *
			normal_velocity.get());
}

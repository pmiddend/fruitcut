#include "parameters.hpp"

fruitcut::fruitlib::physics::rigid_body::parameters::parameters(
	rigid_body::position const &_position,
	rigid_body::transformation const &_transformation,
	rigid_body::linear_velocity const &_linear_velocity,
	rigid_body::angular_velocity const &_angular_velocity,
	physics::shared_shape_ptr const &_shape,
	physics::rigid_body::solidity::type const _solidity,
	rigid_body::optional_mass const &_mass,
	rigid_body::user_data const &_user_data)
:
	position_(
		_position.get()),
	transformation_(
		_transformation.get()),
	linear_velocity_(
		_linear_velocity.get()),
	angular_velocity_(
		_angular_velocity.get()),
	shape_(
		_shape),
	solidity_(
		_solidity),
	mass_(
		_mass),
	user_data_(
		_user_data)
{
}

fruitcut::fruitlib::physics::rigid_body::position::value_type const &
fruitcut::fruitlib::physics::rigid_body::parameters::position() const
{
	return position_;
}

fruitcut::fruitlib::physics::rigid_body::transformation::value_type const &
fruitcut::fruitlib::physics::rigid_body::parameters::transformation() const
{
	return transformation_;
}

fruitcut::fruitlib::physics::rigid_body::linear_velocity::value_type const &
fruitcut::fruitlib::physics::rigid_body::parameters::linear_velocity() const
{
	return linear_velocity_;
}

fruitcut::fruitlib::physics::rigid_body::angular_velocity::value_type const &
fruitcut::fruitlib::physics::rigid_body::parameters::angular_velocity() const
{
	return angular_velocity_;
}

fruitcut::fruitlib::physics::shared_shape_ptr const
fruitcut::fruitlib::physics::rigid_body::parameters::shape() const
{
	return shape_;
}

fruitcut::fruitlib::physics::rigid_body::solidity::type
fruitcut::fruitlib::physics::rigid_body::parameters::solidity() const
{
	return solidity_;
}

fruitcut::fruitlib::physics::rigid_body::optional_mass const &
fruitcut::fruitlib::physics::rigid_body::parameters::mass() const
{
	return mass_;
}

fruitcut::fruitlib::physics::rigid_body::user_data const &
fruitcut::fruitlib::physics::rigid_body::parameters::user_data() const
{
	return user_data_;
}

fruitcut::fruitlib::physics::rigid_body::parameters::~parameters()
{
}

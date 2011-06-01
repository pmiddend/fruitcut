#include "parameters.hpp"

fruitcut::fruitlib::physics::rigid_body::parameters::parameters(
	physics::world &_world,
	vector3 const &_position,
	matrix4 const &_transformation,
	vector3 const &_linear_velocity,
	vector3 const &_angular_velocity,
	shared_shape_ptr const &_shape,
	physics::rigid_body::solidity::type const _solidity,
	fcppt::optional<scalar> const &_mass)
:
	world_(
		_world),
	position_(
		_position),
	transformation_(
		_transformation),
	linear_velocity_(
		_linear_velocity),
	angular_velocity_(
		_angular_velocity),
	shape_(
		_shape),
	solidity_(
		_solidity),
	mass_(
		_mass)
{
}

fruitcut::fruitlib::physics::world &
fruitcut::fruitlib::physics::rigid_body::parameters::world() const
{
	return world_;
}

fruitcut::fruitlib::physics::vector3 const &
fruitcut::fruitlib::physics::rigid_body::parameters::position() const
{
	return position_;
}

fruitcut::fruitlib::physics::matrix4 const &
fruitcut::fruitlib::physics::rigid_body::parameters::transformation() const
{
	return transformation_;
}

fruitcut::fruitlib::physics::vector3 const &
fruitcut::fruitlib::physics::rigid_body::parameters::linear_velocity() const
{
	return linear_velocity_;
}

fruitcut::fruitlib::physics::vector3 const &
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

fcppt::optional<fruitcut::fruitlib::physics::scalar> const &
fruitcut::fruitlib::physics::rigid_body::parameters::mass() const
{
	return mass_;
}

fruitcut::fruitlib::physics::rigid_body::parameters::~parameters()
{
}

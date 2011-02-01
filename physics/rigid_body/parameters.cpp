#include "parameters.hpp"

fruitcut::physics::rigid_body::parameters::parameters(
	fruitcut::physics::world &_world,
	vector3 const &_position,
	matrix4 const &_transformation,
	vector3 const &_linear_velocity,
	shared_shape_ptr const &_shape,
	fruitcut::physics::rigid_body::solidity::type const _solidity,
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
	shape_(
		_shape),
	solidity_(
		_solidity),
	mass_(
		_mass)
{
}

fruitcut::physics::world &
fruitcut::physics::rigid_body::parameters::world() const
{
	return world_;
}

fruitcut::physics::vector3 const &
fruitcut::physics::rigid_body::parameters::position() const
{
	return position_;
}

fruitcut::physics::matrix4 const &
fruitcut::physics::rigid_body::parameters::transformation() const
{
	return transformation_;
}

fruitcut::physics::vector3 const &
fruitcut::physics::rigid_body::parameters::linear_velocity() const
{
	return linear_velocity_;
}

fruitcut::physics::shared_shape_ptr const
fruitcut::physics::rigid_body::parameters::shape() const
{
	return shape_;
}

fruitcut::physics::rigid_body::solidity::type
fruitcut::physics::rigid_body::parameters::solidity() const
{
	return solidity_;
}

fcppt::optional<fruitcut::physics::scalar> const &
fruitcut::physics::rigid_body::parameters::mass() const
{
	return mass_;
}

fruitcut::physics::rigid_body::parameters::~parameters()
{
}

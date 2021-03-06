#ifndef FRUITLIB_PHYSICS_RIGID_BODY_PARAMETERS_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_PARAMETERS_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/physics/matrix4.hpp>
#include <fruitlib/physics/shared_shape_ptr.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fruitlib/physics/rigid_body/angular_velocity.hpp>
#include <fruitlib/physics/rigid_body/linear_velocity.hpp>
#include <fruitlib/physics/rigid_body/optional_mass.hpp>
#include <fruitlib/physics/rigid_body/position.hpp>
#include <fruitlib/physics/rigid_body/solidity.hpp>
#include <fruitlib/physics/rigid_body/transformation.hpp>
#include <fruitlib/physics/rigid_body/user_data.hpp>


namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
class parameters
{
public:
	FRUITLIB_DETAIL_SYMBOL
	explicit
	parameters(
		rigid_body::position const &,
		rigid_body::transformation const &,
		rigid_body::linear_velocity const &,
		rigid_body::angular_velocity const &,
		physics::shared_shape_ptr const &shape,
		physics::rigid_body::solidity::type,
		rigid_body::optional_mass const &,
		rigid_body::user_data const &);

	rigid_body::position::value_type const &
	position() const;

	rigid_body::transformation::value_type const &
	transformation() const;

	rigid_body::linear_velocity::value_type const &
	linear_velocity() const;

	rigid_body::angular_velocity::value_type const &
	angular_velocity() const;

	physics::shared_shape_ptr const
	shape() const;

	physics::rigid_body::solidity::type
	solidity() const;

	rigid_body::optional_mass const &
	mass() const;

	rigid_body::user_data const &
	user_data() const;

	FRUITLIB_DETAIL_SYMBOL
	~parameters();
private:
	rigid_body::position::value_type position_;
	rigid_body::transformation::value_type transformation_;
	rigid_body::linear_velocity::value_type linear_velocity_;
	rigid_body::angular_velocity::value_type angular_velocity_;
	physics::shared_shape_ptr shape_;
	physics::rigid_body::solidity::type solidity_;
	rigid_body::optional_mass mass_;
	rigid_body::user_data user_data_;
};
}
}
}

#endif

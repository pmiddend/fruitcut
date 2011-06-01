#ifndef FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_PARAMETERS_HPP_INCLUDED

#include "solidity.hpp"
#include "../world_fwd.hpp"
#include "../vector3.hpp"
#include "../matrix4.hpp"
#include "../shared_shape_ptr.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/optional.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
class parameters
{
public:
	explicit
	parameters(
		physics::world &,
		vector3 const &position,
		matrix4 const &transformation,
		vector3 const &linear_velocity,
		vector3 const &angular_velocity,
		shared_shape_ptr const &shape,
		physics::rigid_body::solidity::type,
		fcppt::optional<scalar> const &mass);

	physics::world &
	world() const;

	vector3 const &
	position() const;

	matrix4 const &
	transformation() const;

	vector3 const &
	linear_velocity() const;

	vector3 const &
	angular_velocity() const;

	shared_shape_ptr const
	shape() const;

	physics::rigid_body::solidity::type
	solidity() const;

	fcppt::optional<scalar> const &
	mass() const;

	~parameters();
private:
	physics::world &world_;
	vector3 position_;
	matrix4 transformation_;
	vector3 linear_velocity_;
	vector3 angular_velocity_;
	shared_shape_ptr shape_;
	physics::rigid_body::solidity::type solidity_;
	fcppt::optional<scalar> mass_;
};
}
}
}
}

#endif

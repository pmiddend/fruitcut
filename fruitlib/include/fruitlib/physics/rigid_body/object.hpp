#ifndef FRUITLIB_PHYSICS_RIGID_BODY_OBJECT_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_OBJECT_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/physics/shared_shape_ptr.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fruitlib/physics/rigid_body/angular_velocity.hpp>
#include <fruitlib/physics/rigid_body/linear_velocity.hpp>
#include <fruitlib/physics/rigid_body/parameters_fwd.hpp>
#include <fruitlib/physics/rigid_body/position.hpp>
#include <fruitlib/physics/rigid_body/transformation.hpp>
#include <fruitlib/physics/rigid_body/user_data.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


class btRigidBody;
struct btDefaultMotionState;

namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	FRUITLIB_DETAIL_SYMBOL
	explicit
	object(
		fruitlib::physics::rigid_body::parameters const &);

	FRUITLIB_DETAIL_SYMBOL
	fruitlib::physics::rigid_body::transformation const
	world_transform() const;

	FRUITLIB_DETAIL_SYMBOL
	fruitlib::physics::rigid_body::transformation const
	transformation() const;

	FRUITLIB_DETAIL_SYMBOL
	fruitlib::physics::rigid_body::linear_velocity const
	linear_velocity() const;

	FRUITLIB_DETAIL_SYMBOL
	fruitlib::physics::rigid_body::angular_velocity const
	angular_velocity() const;

	FRUITLIB_DETAIL_SYMBOL
	fruitlib::physics::rigid_body::position const
	position() const;

	fruitlib::physics::rigid_body::user_data const &
	user_data() const;

	FRUITLIB_DETAIL_SYMBOL
	~object();
private:
	friend class physics::world;

	fruitlib::physics::shared_shape_ptr shape_;
	std::unique_ptr<btDefaultMotionState> const motion_state_;
	std::unique_ptr<btRigidBody> const body_;
	fruitlib::physics::rigid_body::user_data user_data_;
};
}
}
}

#endif

#ifndef FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_OBJECT_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_OBJECT_HPP_INCLUDED

#include "../shared_shape_ptr.hpp"
#include "../world_fwd.hpp"
#include "transformation.hpp"
#include "linear_velocity.hpp"
#include "angular_velocity.hpp"
#include "position.hpp"
#include "parameters_fwd.hpp"
#include "user_data.hpp"
#include <fcppt/scoped_ptr.hpp>
#include <fcppt/noncopyable.hpp>

class btRigidBody;
struct btDefaultMotionState;

namespace fruitcut
{
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
	explicit
	object(
		rigid_body::parameters const &);

	rigid_body::transformation::value_type const
	world_transform() const;

	rigid_body::transformation::value_type const
	transformation() const;

	rigid_body::linear_velocity::value_type const
	linear_velocity() const;

	rigid_body::angular_velocity::value_type const
	angular_velocity() const;

	rigid_body::position::value_type const
	position() const;

	rigid_body::user_data const &
	user_data() const;

	~object();
private:
	friend class physics::world;

	physics::shared_shape_ptr shape_;
	fcppt::scoped_ptr<btDefaultMotionState> motion_state_;
	fcppt::scoped_ptr<btRigidBody> body_;
	rigid_body::user_data user_data_;
};
}
}
}
}

#endif

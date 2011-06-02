#ifndef FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_OBJECT_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_OBJECT_HPP_INCLUDED

#include "../matrix4.hpp"
#include "../vector3.hpp"
#include "../world_fwd.hpp"
#include "parameters_fwd.hpp"
#include "user_data.hpp"
#include "../shared_shape_ptr.hpp"
#include <fcppt/scoped_ptr.hpp>
#include <fcppt/noncopyable.hpp>

class btRigidBody;
class btDefaultMotionState;

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
		parameters const &);

	matrix4 const
	world_transform() const;

	matrix4 const
	transformation() const;

	vector3	const
	linear_velocity() const;

	vector3	const
	angular_velocity() const;

	vector3 const
	position() const;

	rigid_body::user_data const &
	user_data() const;

	~object();
private:
	friend class physics::world;

	shared_shape_ptr shape_;
	fcppt::scoped_ptr<btDefaultMotionState> motion_state_;
	fcppt::scoped_ptr<btRigidBody> body_;
	rigid_body::user_data user_data_;
};
}
}
}
}

#endif

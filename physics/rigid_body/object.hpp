#ifndef FRUITCUT_PHYSICS_RIGID_BODY_OBJECT_HPP_INCLUDED
#define FRUITCUT_PHYSICS_RIGID_BODY_OBJECT_HPP_INCLUDED

#include "scoped_body.hpp"
#include "../matrix4.hpp"
#include "../vector3.hpp"
#include "parameters_fwd.hpp"
#include "../shared_shape_ptr.hpp"
#include <bullet/LinearMath/btDefaultMotionState.h>
#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
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
	rotation() const;

	vector3	const
	linear_velocity() const;

	vector3	const
	angular_velocity() const;

	vector3 const
	position() const;

	~object();
private:
	shared_shape_ptr shape_;
	btDefaultMotionState motion_state_;
	btRigidBody body_;
	scoped_body world_scope_;
};
}
}
}

#endif

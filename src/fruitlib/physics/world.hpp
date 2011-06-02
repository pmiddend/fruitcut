#ifndef FRUITCUT_FRUITLIB_PHYSICS_WORLD_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_WORLD_HPP_INCLUDED

#include "box.hpp"
#include "vector3.hpp"
#include "scalar.hpp"
#include "group/object_fwd.hpp"
#include "group/id.hpp"
#include "group/sequence.hpp"
#include "rigid_body/collision_callback.hpp"
#include "rigid_body/object_fwd.hpp"
#include "rigid_body/collision_callback_fn.hpp"
#include <sge/time/duration.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/scoped_ptr.hpp>

class btCollisionConfiguration;
class btDispatcher;
class btConstraintSolver;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btDynamicsWorld;

namespace fruitcut
{
namespace fruitlib
{
namespace physics
{
class world
{
FCPPT_NONCOPYABLE(
	world);
public:
	explicit
	world(
		box const &world_size,
		vector3 const &gravity);

	void
	update(
		sge::time::duration const &);

	vector3 const
	gravity() const;

	void
	gravity(
		vector3 const &);

	// The debug drawer needs this
	btDiscreteDynamicsWorld &
	handle();

	void
	add_body(
		rigid_body::object &,
		group::sequence const &);

	void
	remove_body(
		rigid_body::object &);

	void
	make_groups_collide(
		group::object &,
		group::object &);

	fcppt::signal::auto_connection
	rigid_body_collision(
		rigid_body::collision_callback const &);

	~world();
private:
	friend class group::object;

	fcppt::scoped_ptr<btCollisionConfiguration> configuration_;
	fcppt::scoped_ptr<btDispatcher> dispatcher_;
	fcppt::scoped_ptr<btBroadphaseInterface> broadphase_;
	fcppt::scoped_ptr<btConstraintSolver> constraint_solver_;
	fcppt::scoped_ptr<btDiscreteDynamicsWorld> world_;
	group::id next_group_id_;
	fcppt::signal::object<rigid_body::collision_callback_fn> rigid_body_collision_;

	group::id
	next_group_id();

	static void
	internal_tick_callback_static(
		btDynamicsWorld *,
		physics::scalar);

	void
	internal_tick_callback(
		physics::scalar);
};
}
}
}

#endif

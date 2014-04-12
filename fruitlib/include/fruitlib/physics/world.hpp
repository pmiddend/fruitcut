#ifndef FRUITLIB_PHYSICS_WORLD_HPP_INCLUDED
#define FRUITLIB_PHYSICS_WORLD_HPP_INCLUDED

#include <fruitlib/physics/box.hpp>
#include <fruitlib/physics/duration.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/group/id.hpp>
#include <fruitlib/physics/group/object_fwd.hpp>
#include <fruitlib/physics/group/sequence.hpp>
#include <fruitlib/physics/rigid_body/collision_callback.hpp>
#include <fruitlib/physics/rigid_body/collision_callback_fn.hpp>
#include <fruitlib/physics/rigid_body/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


class btCollisionConfiguration;
class btDispatcher;
class btConstraintSolver;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btDynamicsWorld;

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
		physics::duration const &);

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

	std::unique_ptr<btCollisionConfiguration> const configuration_;
	std::unique_ptr<btDispatcher> const dispatcher_;
	std::unique_ptr<btBroadphaseInterface> const broadphase_;
	std::unique_ptr<btConstraintSolver> const constraint_solver_;
	std::unique_ptr<btDiscreteDynamicsWorld> const world_;
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

#endif

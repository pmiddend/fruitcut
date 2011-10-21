#include <fruitlib/physics/world.hpp>
#include <fruitlib/physics/rigid_body/scoped.hpp>
#include <fcppt/config/external_begin.hpp>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <fcppt/config/external_end.hpp>


fruitlib::physics::rigid_body::scoped::scoped(
	world &_world,
	rigid_body::object &_body,
	group::sequence const &_groups)
:
	world_(
		_world),
	body_(
		_body)
{
	world_.add_body(
		body_,
		_groups);
}

fruitlib::physics::rigid_body::scoped::~scoped()
{
	world_.remove_body(
		body_);
}

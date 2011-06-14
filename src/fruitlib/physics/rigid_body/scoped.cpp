#include "scoped.hpp"
#include "../world.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

fruitcut::fruitlib::physics::rigid_body::scoped::scoped(
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

fruitcut::fruitlib::physics::rigid_body::scoped::~scoped()
{
	world_.remove_body(
		body_);
}
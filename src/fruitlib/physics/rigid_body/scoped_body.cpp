#include "scoped_body.hpp"
#include "../world.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

fruitcut::fruitlib::physics::rigid_body::scoped_body::scoped_body(
	world &_world,
	btRigidBody &_body)
:
	world_(
		_world),	
	body_(
		_body)
{
	world_.handle().addRigidBody(
		&body_);
}

fruitcut::fruitlib::physics::rigid_body::scoped_body::~scoped_body()
{
	world_.handle().removeRigidBody(
		&body_);
}

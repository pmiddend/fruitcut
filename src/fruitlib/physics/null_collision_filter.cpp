#include "null_collision_filter.hpp"
#include "world.hpp"
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

fruitcut::fruitlib::physics::null_collision_filter::null_collision_filter(
	world &_world)
:
	world_(
		_world)
{
	world_.handle().getPairCache()->setOverlapFilterCallback(
		this);
}

bool
fruitcut::fruitlib::physics::null_collision_filter::needBroadphaseCollision(
	btBroadphaseProxy *,
	btBroadphaseProxy *) const
{
	return false;
}

fruitcut::fruitlib::physics::null_collision_filter::~null_collision_filter()
{
	world_.handle().getPairCache()->setOverlapFilterCallback(
		0);
}

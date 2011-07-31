#include <fruitlib/physics/null_collision_filter.hpp>
#include <fruitlib/physics/world.hpp>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

fruitlib::physics::null_collision_filter::null_collision_filter(
	world &_world)
:
	world_(
		_world)
{
	world_.handle().getPairCache()->setOverlapFilterCallback(
		this);
}

bool
fruitlib::physics::null_collision_filter::needBroadphaseCollision(
	btBroadphaseProxy *,
	btBroadphaseProxy *) const
{
	return false;
}

fruitlib::physics::null_collision_filter::~null_collision_filter()
{
	world_.handle().getPairCache()->setOverlapFilterCallback(
		0);
}

#include <fruitlib/physics/null_collision_filter.hpp>
#include <fruitlib/physics/world.hpp>
#include <fcppt/config/external_begin.hpp>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <fcppt/config/external_end.hpp>


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
		nullptr
	);
}

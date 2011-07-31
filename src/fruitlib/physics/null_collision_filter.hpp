#ifndef FRUITLIB_PHYSICS_NULL_COLLISION_FILTER_HPP_INCLUDED
#define FRUITLIB_PHYSICS_NULL_COLLISION_FILTER_HPP_INCLUDED

#include "world_fwd.hpp"
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
namespace physics
{
class null_collision_filter
:
	public btOverlapFilterCallback
{
FCPPT_NONCOPYABLE(
	null_collision_filter);
public:
	explicit
	null_collision_filter(
		world &);

	bool
	needBroadphaseCollision(
		btBroadphaseProxy *,
		btBroadphaseProxy *) const;

	~null_collision_filter();
private:
	world &world_;
};
}
}

#endif

#ifndef FRUITCUT_PHYSICS_NULL_COLLISION_FILTER_HPP_INCLUDED
#define FRUITCUT_PHYSICS_NULL_COLLISION_FILTER_HPP_INCLUDED

#include "world_fwd.hpp"
#include <bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
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

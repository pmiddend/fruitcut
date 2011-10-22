#ifndef FRUITLIB_PHYSICS_NULL_COLLISION_FILTER_HPP_INCLUDED
#define FRUITLIB_PHYSICS_NULL_COLLISION_FILTER_HPP_INCLUDED

#include <fruitlib/physics/world_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <fcppt/config/external_end.hpp>


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

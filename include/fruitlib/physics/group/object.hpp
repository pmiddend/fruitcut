#ifndef FRUITLIB_PHYSICS_GROUP_OBJECT_HPP_INCLUDED
#define FRUITLIB_PHYSICS_GROUP_OBJECT_HPP_INCLUDED

#include <fruitlib/physics/group/id.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
namespace physics
{
namespace group
{
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	explicit
	object(
		physics::world &);

	~object();
private:
	friend class physics::world;

	group::id category_,collides_;

	void
	collides_with(
		group::object const &);
};
}
}
}

#endif

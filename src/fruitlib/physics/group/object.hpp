#ifndef FRUITCUT_FRUITLIB_PHYSICS_GROUP_OBJECT_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_GROUP_OBJECT_HPP_INCLUDED

#include "id.hpp"
#include "../world_fwd.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
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
}

#endif
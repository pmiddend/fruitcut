#include "object.hpp"
#include "id.hpp"
#include "../world.hpp"

fruitcut::fruitlib::physics::group::object::object(
	physics::world &_world)
:
	category_(
		_world.next_group_id()),
	collides_(
		// a valid static_cast since id is short! Winning.
		static_cast<group::id>(
			0))
{
}

fruitcut::fruitlib::physics::group::object::~object()
{
}

void
fruitcut::fruitlib::physics::group::object::collides_with(
	group::object const &other)
{
	collides_ |= other.category_;
}

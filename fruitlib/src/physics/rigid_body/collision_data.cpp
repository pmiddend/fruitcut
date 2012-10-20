#include <fruitlib/physics/rigid_body/collision_data.hpp>


fruitlib::physics::rigid_body::collision_data::collision_data(
	rigid_body::user_data const &_body1,
	rigid_body::user_data const &_body2)
:
	body1_(
		_body1),
	body2_(
		_body2)
{
}

fruitlib::physics::rigid_body::user_data const &
fruitlib::physics::rigid_body::collision_data::body1() const
{
	return body1_;
}

fruitlib::physics::rigid_body::user_data const &
fruitlib::physics::rigid_body::collision_data::body2() const
{
	return body2_;
}

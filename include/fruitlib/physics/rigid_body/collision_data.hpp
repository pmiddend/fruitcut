#ifndef FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_DATA_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_COLLISION_DATA_HPP_INCLUDED

#include <fruitlib/physics/rigid_body/user_data.hpp>

namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
class collision_data
{
public:
	explicit
	collision_data(
		rigid_body::user_data const &,
		rigid_body::user_data const &);

	rigid_body::user_data const &
	body1() const;

	rigid_body::user_data const &
	body2() const;
private:
	rigid_body::user_data body1_,body2_;
};
}
}
}

#endif

#ifndef FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_SCOPED_BODY_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_SCOPED_BODY_HPP_INCLUDED

#include "../world_fwd.hpp"

class btRigidBody;

namespace fruitcut
{
namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
class scoped_body
{
public:
	explicit
	scoped_body(
		world &,
		btRigidBody &);

	~scoped_body();
private:
	world &world_;
	btRigidBody &body_;
};
}
}
}
}

#endif

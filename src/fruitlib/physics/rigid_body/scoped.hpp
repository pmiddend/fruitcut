#ifndef FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_SCOPED_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_SCOPED_HPP_INCLUDED

#include "../world_fwd.hpp"
#include "object_fwd.hpp"
#include "../group/sequence.hpp"

namespace fruitcut
{
namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
class scoped
{
public:
	explicit
	scoped(
		physics::world &,
		rigid_body::object &,
		group::sequence const &);

	~scoped();
private:
	world &world_;
	rigid_body::object &body_;
};
}
}
}
}

#endif

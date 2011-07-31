#ifndef FRUITLIB_PHYSICS_RIGID_BODY_SCOPED_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_SCOPED_HPP_INCLUDED

#include "../world_fwd.hpp"
#include "object_fwd.hpp"
#include "../group/sequence.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
class scoped
{
FCPPT_NONCOPYABLE(
	scoped);
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

#endif

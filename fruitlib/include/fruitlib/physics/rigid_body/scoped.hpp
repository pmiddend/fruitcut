#ifndef FRUITLIB_PHYSICS_RIGID_BODY_SCOPED_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_SCOPED_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fruitlib/physics/group/sequence.hpp>
#include <fruitlib/physics/rigid_body/object_fwd.hpp>
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
	FRUITLIB_DETAIL_SYMBOL
	explicit
	scoped(
		physics::world &,
		rigid_body::object &,
		group::sequence const &);

	FRUITLIB_DETAIL_SYMBOL
	~scoped();
private:
	world &world_;
	rigid_body::object &body_;
};
}
}
}

#endif

#ifndef FRUITCUT_FRUITLIB_PHYSICS_NODES_WORLD_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_NODES_WORLD_HPP_INCLUDED

#include "../../scenic/nodes/intrusive.hpp"
#include "../world_fwd.hpp"
#include <sge/time/timer.hpp>
#include <sge/time/callback.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace physics
{
namespace nodes
{
class world
:
	public scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	world);
public:
	explicit
	world(
		physics::world &,
		sge::time::callback const &);

	~world();
protected:
	physics::world &world_;
	sge::time::timer timer_;

	void
	update();

	void
	render();
};
}
}
}
}

#endif

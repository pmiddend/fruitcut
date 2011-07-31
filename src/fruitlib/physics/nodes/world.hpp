#ifndef FRUITLIB_PHYSICS_NODES_WORLD_HPP_INCLUDED
#define FRUITLIB_PHYSICS_NODES_WORLD_HPP_INCLUDED

#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update_fwd.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/callback.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitlib
{
namespace physics
{
namespace nodes
{
class world
:
	public scenic::node<world>
{
FCPPT_NONCOPYABLE(
	world);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	world(
		fruitlib::scenic::optional_parent const &,
		physics::world &,
		sge::time::callback const &);

	~world();

	void
	react(
		fruitlib::scenic::events::update const &);
protected:
	physics::world &world_;
	sge::time::timer timer_;
};
}
}
}

#endif

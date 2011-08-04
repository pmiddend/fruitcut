#ifndef FRUITLIB_PHYSICS_NODES_WORLD_HPP_INCLUDED
#define FRUITLIB_PHYSICS_NODES_WORLD_HPP_INCLUDED

#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update_fwd.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/timer.hpp>
#include <fruitlib/scenic/delta/clock.hpp>
#include <fruitlib/physics/world_fwd.hpp>
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
		scenic::delta::callback const &,
		physics::world &);

	~world();

	void
	react(
		fruitlib::scenic::events::update const &);
protected:
	scenic::delta::clock clock_;
	scenic::delta::timer timer_;
	physics::world &world_;
};
}
}
}

#endif

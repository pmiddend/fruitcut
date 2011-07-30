#ifndef FRUITCUT_FRUITLIB_PHYSICS_NODES_WORLD_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_NODES_WORLD_HPP_INCLUDED

#include "../../scenic/node.hpp"
#include "../../scenic/parent_fwd.hpp"
#include "../../scenic/events/update_fwd.hpp"
#include "../world_fwd.hpp"
#include <sge/time/timer.hpp>
#include <sge/time/callback.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

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
		fruitlib::scenic::parent const &,
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
}

#endif

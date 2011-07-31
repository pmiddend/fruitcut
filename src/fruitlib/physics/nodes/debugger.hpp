#ifndef FRUITCUT_FRUITLIB_PHYSICS_NODES_DEBUGGER_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_NODES_DEBUGGER_HPP_INCLUDED

#include "../debugger_fwd.hpp"
#include "../../scenic/node.hpp"
#include "../../scenic/optional_parent.hpp"
#include "../../scenic/events/update_fwd.hpp"
#include "../../scenic/events/render_fwd.hpp"
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
class debugger
:
	public scenic::node<debugger>
{
FCPPT_NONCOPYABLE(
	debugger);
public:
	typedef
	boost::mpl::vector2
	<
		fruitlib::scenic::events::render,
		fruitlib::scenic::events::update
	>
	scene_reactions;

	explicit
	debugger(
		fruitlib::scenic::optional_parent const &,
		physics::debugger &);

	~debugger();

	void
	react(
		fruitlib::scenic::events::render const &);

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	physics::debugger &debugger_;
};
}
}
}
}

#endif

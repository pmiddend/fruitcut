#ifndef FRUITLIB_PHYSICS_NODES_DEBUGGER_HPP_INCLUDED
#define FRUITLIB_PHYSICS_NODES_DEBUGGER_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/physics/debugger_fwd.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace physics
{
namespace nodes
{
class debugger
:
	public fruitlib::scenic::node<debugger>
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

	FRUITLIB_DETAIL_SYMBOL
	explicit
	debugger(
		fruitlib::scenic::optional_parent const &,
		fruitlib::physics::debugger &);

	FRUITLIB_DETAIL_SYMBOL
	~debugger();

	void
	react(
		fruitlib::scenic::events::render const &);

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	fruitlib::physics::debugger &debugger_;
};
}
}
}

#endif

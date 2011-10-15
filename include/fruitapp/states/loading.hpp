#ifndef FRUITAPP_STATES_LOADING_HPP_INCLUDED
#define FRUITAPP_STATES_LOADING_HPP_INCLUDED

#include <fruitapp/states/menu/superstate_fwd.hpp>
#include <fruitapp/machine.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <fruitlib/font/scene_node.hpp>
#include <sge/parse/json/element_vector.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
namespace states
{
class loading
:
	// The second argument has to be a complete type
	public boost::statechart::state<loading,machine>,
	public fruitlib::scenic::node<loading>
{
FCPPT_NONCOPYABLE(
	loading);
public:
	typedef
	boost::mpl::vector1
	<
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::superstate)
	>
	reactions;

	typedef
	boost::mpl::vector2<fruitlib::scenic::events::update,fruitlib::scenic::events::viewport_change>
	scene_reactions;

	explicit
	loading(
		my_context);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::superstate);

	~loading();

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	react(
		fruitlib::scenic::events::viewport_change const &);
private:
	sge::parse::json::element_vector const fruit_array_;
	sge::parse::json::element_vector::const_iterator current_fruit_;
	fruitlib::font::scene_node font_node_;
};
}
}

#endif

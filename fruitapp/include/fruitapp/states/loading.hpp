#ifndef FRUITAPP_STATES_LOADING_HPP_INCLUDED
#define FRUITAPP_STATES_LOADING_HPP_INCLUDED

#include <fruitapp/machine.hpp>
#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/states/menu/superstate_fwd.hpp>
#include <fruitlib/font/scene_node.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/parse/json/element_vector.hpp>
#include <sge/renderer/target/viewport.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/state.hpp>
#include <fcppt/config/external_end.hpp>


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
	boost::mpl::vector1<fruitlib::scenic::events::update>
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
private:
	sge::parse::json::element_vector const &fruit_array_;
	sge::parse::json::element_vector::const_iterator current_fruit_;
	fruitlib::font::scene_node font_node_;
	fcppt::signal::scoped_connection viewport_change_connection_;

	void
	viewport_change(
		sge::renderer::target::viewport const &);
};
}
}

#endif

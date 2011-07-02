#ifndef FRUITCUT_APP_STATES_LOADING_HPP_INCLUDED
#define FRUITCUT_APP_STATES_LOADING_HPP_INCLUDED

#include "menu/superstate_fwd.hpp"
#include "../machine.hpp"
#include "../events/declare_transition_type.hpp"
#include "../events/declare_transition_reaction.hpp"
#include "../../fruitlib/scenic/nodes/intrusive.hpp"
#include "../../fruitlib/font/intrusive_scene_node.hpp"
#include <sge/renderer/state/scoped.hpp>
#include <sge/parse/json/json.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
class loading
:
	// The second argument has to be a complete type
	public boost::statechart::state<loading,machine>,
	public fruitlib::scenic::nodes::intrusive
{
public:
	typedef
	boost::mpl::vector1
	<
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::superstate)
	>
	reactions;

	explicit
	loading(
		my_context);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::superstate);

	~loading();
private:
	sge::renderer::state::scoped scoped_render_state_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	sge::parse::json::element_vector const fruit_array_;
	sge::parse::json::element_vector::const_iterator current_fruit_;
	fruitlib::font::intrusive_scene_node font_node_;

	void
	viewport_change();

	void
	update();

	void
	render();
};
}
}
}

#endif

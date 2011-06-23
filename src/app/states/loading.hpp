#ifndef FRUITCUT_APP_STATES_LOADING_HPP_INCLUDED
#define FRUITCUT_APP_STATES_LOADING_HPP_INCLUDED

#include "menu_fwd.hpp"
#include "../machine.hpp"
#include "../events/make_transition.hpp"
#include "../../fruitlib/scenic/nodes/intrusive.hpp"
#include "../../fruitlib/font/intrusive_scene_node.hpp"
#include <sge/renderer/state/scoped.hpp>
#include <sge/parse/json/json.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/statechart/state.hpp>

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
		events::make_transition<states::menu>::type
	>
	reactions;

	explicit
	loading(
		my_context);

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

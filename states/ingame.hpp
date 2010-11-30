#ifndef FRUITCUT_STATES_INGAME_HPP_INCLUDED
#define FRUITCUT_STATES_INGAME_HPP_INCLUDED

#include "freelook_fwd.hpp"
#include "../machine.hpp"
#include "../input_states.hpp"
#include "../events/render_overlay.hpp"
#include "../events/render.hpp"
#include "../mesh.hpp"
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/camera/object.hpp>
#include <sge/console/object.hpp>
#include <sge/console/gfx.hpp>
#include <sge/console/arg_list.hpp>
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/shader/object.hpp>
#include <sge/model/object_ptr.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/ptr_container/ptr_array.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <cstddef>

namespace fruitcut
{
namespace states
{
class ingame
:
	public boost::statechart::state<ingame,machine,freelook>
{
public:
	typedef 
	boost::mpl::vector2
	<
		boost::statechart::custom_reaction<events::render_overlay>,
		boost::statechart::custom_reaction<events::render>
	> 
	reactions;

	explicit
	ingame(
		my_context);

	// Ingame needs to update the camera
	sge::camera::object &
	camera();

	void
	current_input_state(
		input_states::type);

	boost::statechart::result
	react(
		events::render const &);

	boost::statechart::result
	react(
		events::render_overlay const &);

	// Called by the...you guessed it - cut state!
	void
	cut(
		sge::renderer::vector3 const &,
		sge::renderer::vector3 const &,
		sge::renderer::vector3 const &);
private:
	typedef
	boost::ptr_array
	<
		input::state,
		static_cast<std::size_t>(input_states::size)
	>
	input_state_map;

	input::state_manager input_manager_;
	input_state_map input_states_;
	input_states::type current_input_state_;
	sge::camera::object camera_;
	sge::console::object console_;
	sge::console::gfx console_gfx_;
	fcppt::signal::scoped_connection console_connection_;
	fcppt::signal::scoped_connection state_change_connection_;
	bool wireframe_;
	fcppt::signal::scoped_connection wireframe_connection_;

	// game relevant stuff
	sge::model::object_ptr model_;
	sge::renderer::texture_ptr model_texture_;
	sge::shader::object shader_;
	mesh mesh_;
	sge::renderer::vertex_buffer_ptr mesh_vb_;
	sge::renderer::state::scoped scoped_state_;
	sge::renderer::vector3 mesh_translation_;
	sge::renderer::matrix4 mesh_rotation_;

	void
	console_callback(
		sge::input::keyboard::key_event const &);

	void
	toggle_mode(
		sge::console::arg_list const &,
		sge::console::object &);

	void
	toggle_wireframe(
		sge::console::arg_list const &,
		sge::console::object &);
};
}
}

#endif

#ifndef FRUITCUT_APP_MACHINE_HPP_INCLUDED
#define FRUITCUT_APP_MACHINE_HPP_INCLUDED

#include "background.hpp"
#include "postprocessing.hpp"
#include "../input/state.hpp"
#include "../input/state_manager.hpp"
#include "../particle/system.hpp"
#include "../sound_controller.hpp"
#include "states/intro_fwd.hpp"
#include <awl/mainloop/asio/io_service_ptr.hpp>
#include <sge/console/gfx.hpp>
#include <sge/console/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/systems/instance.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/point.hpp>
#include <sge/time/unit.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/chrono/time_point.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/function.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/system/error_code.hpp>

namespace fruitcut
{
namespace app
{
class machine
:
	public 
		boost::statechart::state_machine
		<
			machine,
			states::intro
		>
{
public:
	typedef
	boost::function<sge::time::duration const (sge::time::duration const &)>
	time_transform_function;

	explicit
	machine(
		int argc,
		char *argv[]);

	sge::parse::json::object const &
	config_file() const;

	sge::systems::instance const &
	systems() const;

	// Needed for the viewport callback stuff
	sge::systems::instance &
	systems();

	particle::system &
	particle_system();

	sge::texture::part_ptr const
	create_single_texture(
		fcppt::filesystem::path const &,
		sge::renderer::texture::address_mode::type);

	sge::texture::part_ptr const
	create_texture(
		fcppt::filesystem::path const &);

	void
	run();

	fruitcut::app::postprocessing &
	postprocessing();

	sge::time::callback const 
	timer_callback() const;

	fruitcut::sound_controller &
	sound_controller();

	fruitcut::sound_controller const &
	sound_controller() const;

	input::state &
	game_input_state();

	input::state_manager &
	input_manager();

	fruitcut::app::background &
	background();

	fruitcut::app::background const &
	background() const;

	~machine();
private:
	sge::parse::json::object const config_file_;
	awl::mainloop::asio::io_service_ptr io_service_;
	// This is nonconst because of manage_viewport_callback, which is
	// nonconst (this might be a bug, though)
	sge::systems::instance systems_;
	sge::texture::manager texture_manager_;
	input::state_manager input_manager_;
	input::state console_state_,game_state_;
	input::state *previous_state_;
	sge::console::object console_object_;
	sge::console::gfx console_gfx_;
	fruitcut::app::postprocessing postprocessing_;
	particle::system particle_system_;
	fcppt::signal::scoped_connection exit_connection_;
	sge::time::point current_time_,transformed_time_;
	time_transform_function time_transform_;
	fcppt::signal::scoped_connection console_switch_connection_;
	fruitcut::sound_controller sound_controller_;
	boost::asio::deadline_timer frame_timer_;
	fruitcut::app::background background_;
	fcppt::signal::scoped_connection viewport_change_connection_;

	void
	console_switch();

	void
	run_once(
		boost::system::error_code const &);

	void
	viewport_change(
		sge::renderer::device_ptr);

	void
	manage_time();

	void 
	manage_rendering();
};
}
}

#endif

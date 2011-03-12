#ifndef FRUITCUT_APP_MACHINE_HPP_INCLUDED
#define FRUITCUT_APP_MACHINE_HPP_INCLUDED

#include "background.hpp"
#include "postprocessing.hpp"
#include "score.hpp"
#include "../input/state.hpp"
#include "../input/state_manager.hpp"
#include "../particle/system.hpp"
#include "../sound_controller.hpp"
#include "../font/cache.hpp"
#include "states/intro_fwd.hpp"
#include <sge/console/gfx.hpp>
#include <sge/console/object.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/syringe.hpp>
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
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/signal/scoped_connection.hpp>
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

	font::cache &
	font_cache();

	font::cache const &
	font_cache() const;

	sge::cegui::system &
	gui_system();

	sge::cegui::system const &
	gui_system() const;

	sge::cegui::syringe &
	gui_syringe();

	sge::cegui::syringe const &
	gui_syringe() const;

	score
	last_game_score() const;

	void
	last_game_score(
		score);

	void
	quit();

	~machine();
private:
	bool running_;
	sge::parse::json::object const config_file_;
	// This is nonconst because of manage_viewport_callback, which is
	// nonconst (this might be a bug, though)
	sge::systems::instance systems_;
	font::cache font_cache_;
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
	fruitcut::app::background background_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	fcppt::chrono::milliseconds::rep desired_fps_;
	sge::cegui::system gui_system_;
	sge::cegui::syringe gui_syringe_;
	score last_game_score_;

	void
	console_switch();

	void
	run_once();

	void
	viewport_change();

	void
	manage_time();

	void 
	manage_rendering();
};
}
}

#endif

#ifndef FRUITCUT_APP_MACHINE_HPP_INCLUDED
#define FRUITCUT_APP_MACHINE_HPP_INCLUDED

#include "../audio/music_controller.hpp"
#include "../audio/sound_controller.hpp"
#include "background.hpp"
#include "../font/cache.hpp"
#include "../input/state.hpp"
#include "../input/state_manager.hpp"
#include "../log/scoped_sequence_ptr.hpp"
#include "overlay.hpp"
#include "scene.hpp"
#include "../scenic/nodes/console.hpp"
#include "../scenic/nodes/intrusive_group.hpp"
#include "../scenic/nodes/music_controller.hpp"
#include "../scenic/nodes/sound_controller.hpp"
#include "score.hpp"
#include "states/intro_fwd.hpp"
#include <sge/console/gfx.hpp>
#include <sge/console/object.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/syringe.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/systems/instance.hpp>
#include <sge/time/duration.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/time/point.hpp>
#include <sge/time/unit.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/chrono/time_point.hpp>
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/signal/scoped_connection.hpp>
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
		>,
	public 
		scenic::nodes::intrusive_group
{
public:
	typedef
	std::tr1::function<sge::time::duration const (sge::time::duration const &)>
	time_transform_function;

	explicit
	machine(
		int argc,
		char *argv[]);

	sge::parse::json::object const &
	config_file() const;

	sge::systems::instance const &
	systems() const;

	void
	run();

	fruitcut::app::postprocessing &
	postprocessing();

	sge::time::callback const 
	timer_callback() const;

	fruitcut::audio::sound_controller &
	sound_controller();

	fruitcut::audio::sound_controller const &
	sound_controller() const;

	fruitcut::audio::music_controller &
	music_controller();

	fruitcut::audio::music_controller const &
	music_controller() const;

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

	scene &
	scene_node();

	scene const &
	scene_node() const;

	overlay &
	overlay_node();

	overlay const &
	overlay_node() const;

	~machine();
private:
	bool running_;
	sge::parse::json::object const config_file_;
	sge::systems::instance const systems_;
	sge::console::object console_object_;
	scene scene_node_;
	overlay overlay_node_;
	log::scoped_sequence_ptr activated_loggers_;
	font::cache font_cache_;
	input::state_manager input_manager_;
	input::state console_state_,game_state_;
	input::state *previous_state_;
	sge::font::metrics_ptr console_font_;
	sge::console::gfx console_gfx_;
	scenic::nodes::console console_node_;
	fcppt::signal::scoped_connection exit_connection_;
	sge::time::point current_time_,transformed_time_;
	time_transform_function time_transform_;
	fcppt::signal::scoped_connection console_switch_connection_;
	fruitcut::audio::sound_controller sound_controller_;
	fruitcut::scenic::nodes::sound_controller sound_controller_node_;
	fruitcut::audio::music_controller music_controller_;
	fruitcut::scenic::nodes::music_controller music_controller_node_;
	fruitcut::app::background background_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	fcppt::chrono::milliseconds::rep desired_fps_;
	sge::cegui::system gui_system_;
	sge::cegui::syringe gui_syringe_;
	score last_game_score_;

	void
	console_switch();

	void
	viewport_change();

	void 
	manage_rendering();

	// @override
	void
	update();

	// @override
	void
	render();
};
}
}

#endif

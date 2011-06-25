#ifndef FRUITCUT_APP_MACHINE_IMPL_HPP_INCLUDED
#define FRUITCUT_APP_MACHINE_IMPL_HPP_INCLUDED

#include "score.hpp"
#include "background.hpp"
#include "directional_light_source.hpp"
#include "../fruitlib/audio/music_controller.hpp"
#include "../fruitlib/audio/sound_controller.hpp"
#include "../fruitlib/font/cache.hpp"
#include "../fruitlib/input/state.hpp"
#include "../fruitlib/input/state.hpp"
#include "../fruitlib/input/state_manager.hpp"
#include "../fruitlib/log/scoped_sequence_ptr.hpp"
#include "../fruitlib/rng_creator.hpp"
#include "../fruitlib/scenic/nodes/camera.hpp"
#include "../fruitlib/scenic/nodes/console.hpp"
#include "../fruitlib/scenic/nodes/intrusive_group.hpp"
#include "../fruitlib/scenic/nodes/music_controller.hpp"
#include "../fruitlib/scenic/nodes/sound_controller.hpp"
#include "../fruitlib/screen_shooter.hpp"
#include "overlay.hpp"
#include "point_sprite/system_node.hpp"
#include "scene.hpp"
#include "shadow_map.hpp"
#include "fruit/prototype_sequence.hpp"
#include <sge/camera/object.hpp>
#include <sge/cegui/syringe.hpp>
#include <sge/cegui/system.hpp>
#include <sge/console/gfx.hpp>
#include <sge/console/object.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/systems/instance.hpp>
#include <sge/time/time.hpp>
#include <fcppt/chrono/chrono.hpp>

namespace fruitcut
{
namespace app
{
class machine_impl
:
	public fruitlib::scenic::nodes::intrusive_group
{
FCPPT_NONCOPYABLE(
	machine_impl);
public:
	explicit
	machine_impl(
		int argc,
		char *argv[]);

	sge::parse::json::object const &
	config_file() const;

	sge::parse::json::object &
	user_config_file();

	sge::parse::json::object const &
	user_config_file() const;

	sge::systems::instance const &
	systems() const;

	void
	run_once();

	fruitcut::app::postprocessing &
	postprocessing();

	sge::time::callback const 
	timer_callback() const;

	fruitlib::audio::sound_controller &
	sound_controller();

	fruitlib::audio::sound_controller const &
	sound_controller() const;

	fruitlib::audio::music_controller &
	music_controller();

	fruitlib::audio::music_controller const &
	music_controller() const;

	fruitlib::input::state &
	game_input_state();

	fruitlib::input::state_manager &
	input_manager();

	fruitcut::app::background &
	background();

	fruitcut::app::background const &
	background() const;

	fruitcut::app::directional_light_source const &
	main_light_source();

	fruitcut::app::shadow_map &
	shadow_map();

	fruitcut::app::shadow_map const &
	shadow_map() const;

	sge::camera::object &
	camera();

	sge::camera::object const &
	camera() const;

	fruitlib::font::cache &
	font_cache();

	fruitlib::font::cache const &
	font_cache() const;

	sge::cegui::system &
	gui_system();

	sge::cegui::system const &
	gui_system() const;

	sge::cegui::syringe &
	gui_syringe();

	sge::cegui::syringe const &
	gui_syringe() const;

	fruitlib::rng_creator &
	rng_creator();

	score
	last_game_score() const;

	void
	last_game_score(
		score const &);

	scene &
	scene_node();

	scene const &
	scene_node() const;

	overlay &
	overlay_node();

	overlay const &
	overlay_node() const;

	point_sprite::system_node &
	point_sprites();

	point_sprite::system_node const &
	point_sprites() const;

	sge::time::funit
	time_factor() const;

	void
	time_factor(
		sge::time::funit);

	fruit::prototype_sequence const &
	fruit_prototypes() const;

	fruit::prototype_sequence &
	fruit_prototypes();

	~machine_impl();
private:
	fruitlib::rng_creator rng_creator_;
	sge::parse::json::object user_config_file_;
	sge::parse::json::object const config_file_;
	sge::systems::instance const systems_;
	sge::console::object console_object_;
	app::scene scene_node_;
	app::overlay overlay_node_;
	fruitlib::log::scoped_sequence_ptr activated_loggers_;
	fruitlib::font::cache font_cache_;
	fruitlib::input::state_manager input_manager_;
	fruitlib::input::state console_state_,game_state_;
	fruitlib::input::state *previous_state_;
	sge::font::metrics_ptr console_font_;
	sge::console::gfx console_gfx_;
	fruitlib::scenic::nodes::console console_node_;
	fcppt::signal::scoped_connection exit_connection_;
	sge::time::point current_time_,transformed_time_;
	sge::time::funit time_factor_;
	fcppt::signal::scoped_connection console_switch_connection_;
	fruitlib::audio::sound_controller sound_controller_;
	fruitlib::scenic::nodes::sound_controller sound_controller_node_;
	fruitlib::audio::music_controller music_controller_;
	fruitlib::scenic::nodes::music_controller music_controller_node_;
	sge::camera::object camera_;
	fruitlib::scenic::nodes::camera camera_node_;
	fcppt::signal::scoped_connection toggle_camera_connection_;
	fruitlib::input::state camera_state_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	app::directional_light_source main_light_source_;
	fruitcut::app::shadow_map shadow_map_;
	fruitcut::app::background background_;
	fcppt::chrono::milliseconds::rep desired_fps_;
	sge::cegui::system gui_system_;
	sge::cegui::syringe gui_syringe_;
	app::score last_game_score_;
	point_sprite::system_node point_sprites_;
	fruitlib::screen_shooter screen_shooter_;
	fruit::prototype_sequence fruit_prototypes_;

	void
	console_switch();

	void
	toggle_camera();

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
#ifndef FRUITAPP_MACHINE_IMPL_HPP_INCLUDED
#define FRUITAPP_MACHINE_IMPL_HPP_INCLUDED

#include <fruitapp/highscore/score.hpp>
#include <fruitapp/background.hpp>
#include <fruitapp/directional_light_source.hpp>
#include <fruitapp/config_variables.hpp>
#include <fruitapp/quick_log.hpp>
#include <fruitlib/audio/music_controller.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/font/cache.hpp>
#include <fruitlib/log/scoped_sequence_ptr.hpp>
#include <fruitlib/random_generator.hpp>
#include <fruitlib/scenic/adaptors/camera.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/duration.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitapp/screen_shooter.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitapp/renderable.hpp>
#include <fruitapp/shadow_map.hpp>
#include <fruitapp/fruit/prototype_sequence.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <sge/camera/object.hpp>
#include <sge/cegui/syringe.hpp>
#include <sge/cegui/system.hpp>
#include <sge/model/md3/loader_ptr.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/systems/instance.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <fcppt/chrono/milliseconds.hpp>

namespace fruitapp
{
class machine_impl
:
	public fruitlib::scenic::node<machine_impl>
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

	sge::systems::instance const &
	systems() const;

	sge::model::md3::loader &
	md3_loader();

	fruitapp::config_variables &
	config_variables();

	fruitapp::config_variables const &
	config_variables() const;

	void
	run_once();

	fruitapp::postprocessing &
	postprocessing();

	fruitapp::ingame_clock const &
	ingame_clock() const;

	fruitlib::audio::sound_controller &
	sound_controller();

	fruitlib::audio::sound_controller const &
	sound_controller() const;

	fruitlib::audio::music_controller &
	music_controller();

	fruitlib::audio::music_controller const &
	music_controller() const;

	fruitapp::background &
	background();

	fruitapp::background const &
	background() const;

	fruitapp::directional_light_source const &
	main_light_source();

	fruitapp::shadow_map &
	shadow_map();

	fruitapp::shadow_map const &
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

	fruitlib::random_generator &
	random_generator();

	highscore::score::value_type
	last_game_score() const;

	void
	last_game_score(
		highscore::score::value_type const &);

	fruitlib::scenic::base &
	root_node();

	fruitlib::scenic::base const &
	root_node() const;

	scene &
	scene_node();

	scene const &
	scene_node() const;

	overlay &
	overlay_node();

	overlay const &
	overlay_node() const;

	fruitlib::scenic::delta::callback const
	ingame_clock_callback() const;
	
	fruitlib::scenic::delta::callback const
	standard_clock_callback() const;

	point_sprite::system_node &
	point_sprites();

	point_sprite::system_node const &
	point_sprites() const;

	fruitapp::ingame_clock::float_type
	time_factor() const;

	void
	time_factor(
		fruitapp::ingame_clock::float_type);

	fruit::prototype_sequence const &
	fruit_prototypes() const;

	fruit::prototype_sequence &
	fruit_prototypes();

	fruitapp::quick_log &
	quick_log();

	~machine_impl();
private:
	fruitlib::random_generator random_generator_;
	sge::parse::json::object user_config_file_;
	sge::parse::json::object const config_file_;
	fruitapp::config_variables config_variables_;
	sge::systems::instance const systems_;
	sge::model::md3::loader_ptr md3_loader_;
	fruitapp::renderable renderable_;
	fruitlib::log::scoped_sequence_ptr activated_loggers_;
	fruitlib::font::cache font_cache_;
	sge::timer::basic<sge::timer::clocks::standard> second_timer_;
	fruitapp::ingame_clock ingame_clock_;
	fruitlib::scenic::delta::duration ingame_clock_delta_;
	fruitlib::scenic::delta::duration standard_clock_delta_;
	fruitlib::audio::sound_controller sound_controller_;
	fcppt::signal::scoped_connection effects_volume_change_connection_;
	fruitlib::audio::music_controller music_controller_;
	fcppt::signal::scoped_connection music_volume_change_connection_;
	fruitapp::quick_log quick_log_;
	sge::camera::object camera_;
	fruitlib::scenic::adaptors::camera camera_node_;
	fcppt::signal::scoped_connection toggle_camera_connection_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	fruitapp::directional_light_source main_light_source_;
	fruitapp::shadow_map shadow_map_;
	fruitapp::background background_;
	fcppt::chrono::milliseconds::rep desired_fps_;
	sge::cegui::system gui_system_;
	sge::cegui::syringe gui_syringe_;
	highscore::score::value_type last_game_score_;
	point_sprite::system_node point_sprites_;
	fruitapp::screen_shooter screen_shooter_;
	fruit::prototype_sequence fruit_prototypes_;

	void
	toggle_camera();

	void
	viewport_change();

	void 
	manage_rendering();
};
}

#endif

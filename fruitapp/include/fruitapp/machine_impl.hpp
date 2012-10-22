#ifndef FRUITAPP_MACHINE_IMPL_HPP_INCLUDED
#define FRUITAPP_MACHINE_IMPL_HPP_INCLUDED

#include <fruitlib/scoped_ostream_file_redirection.hpp>
#include <fruitlib/scoped_wostream_file_redirection.hpp>
#include <fruitapp/background.hpp>
#include <fruitapp/config_variables.hpp>
#include <fcppt/config/platform.hpp>
#include <fruitapp/directional_light_source.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/quick_log.hpp>
#include <fruitapp/renderable.hpp>
#include <fruitapp/screen_shooter.hpp>
#include <fruitapp/shadow_map.hpp>
#include <fruitapp/systems.hpp>
#include <fruitapp/fruit/prototype_sequence.hpp>
#include <fruitapp/gui/system_unique_ptr.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitapp/projection_manager/object.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/random_generator.hpp>
#include <fruitlib/audio/music_controller.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/font/cache.hpp>
#include <fruitlib/log/scoped_sequence_ptr.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/adaptors/camera.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/duration.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/camera/first_person/object.hpp>
#include <sge/charconv/system_scoped_ptr.hpp>
#include <sge/model/md3/loader_fwd.hpp>
#include <sge/model/md3/loader_scoped_ptr.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/shader/context.hpp>
#include <sge/systems/instance.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <awl/main/exit_code.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>
#include <fcppt/signal/scoped_connection.hpp>


namespace fruitapp
{
class machine_impl
:
	public fruitlib::scenic::node<machine_impl>
{
FCPPT_NONCOPYABLE(
	machine_impl);
public:
	machine_impl(
		int argc,
		char *argv[]);

	sge::parse::json::object const &
	config_file() const
	FCPPT_PP_CONST;

	fruitapp::systems const &
	systems() const
	FCPPT_PP_CONST;

	sge::shader::context &
	shader_context();

	sge::model::md3::loader &
	md3_loader()
	FCPPT_PP_CONST;

	fruitapp::config_variables &
	config_variables()
	FCPPT_PP_CONST;

	fruitapp::config_variables const &
	config_variables() const
	FCPPT_PP_CONST;

	bool
	run_once()
	FCPPT_PP_WARN_UNUSED_RESULT;

	void
	quit(
		awl::main::exit_code);

	awl::main::exit_code const
	exit_code() const
	FCPPT_PP_CONST;

	fruitapp::postprocessing &
	postprocessing()
	FCPPT_PP_CONST;

	fruitapp::ingame_clock const &
	ingame_clock() const
	FCPPT_PP_CONST;

	fruitlib::audio::sound_controller &
	sound_controller()
	FCPPT_PP_CONST;

	fruitlib::audio::sound_controller const &
	sound_controller() const
	FCPPT_PP_CONST;

	fruitlib::audio::music_controller &
	music_controller()
	FCPPT_PP_CONST;

	fruitlib::audio::music_controller const &
	music_controller() const
	FCPPT_PP_CONST;

	fruitapp::background &
	background()
	FCPPT_PP_CONST;

	fruitapp::background const &
	background() const
	FCPPT_PP_CONST;

	fruitapp::directional_light_source const &
	main_light_source()
	FCPPT_PP_CONST;

	fruitapp::shadow_map &
	shadow_map()
	FCPPT_PP_CONST;

	fruitapp::shadow_map const &
	shadow_map() const
	FCPPT_PP_CONST;

	sge::camera::first_person::object &
	camera()
	FCPPT_PP_CONST;

	sge::camera::first_person::object const &
	camera() const
	FCPPT_PP_CONST;

	fruitlib::font::cache &
	font_cache()
	FCPPT_PP_CONST;

	fruitlib::font::cache const &
	font_cache() const
	FCPPT_PP_CONST;

	fruitapp::gui::system &
	gui_system();

	fruitlib::random_generator &
	random_generator()
	FCPPT_PP_CONST;

	highscore::score::value_type
	last_game_score() const
	FCPPT_PP_CONST;

	void
	last_game_score(
		highscore::score::value_type const &);

	fruitlib::scenic::base &
	root_node()
	FCPPT_PP_CONST;

	fruitlib::scenic::base const &
	root_node() const
	FCPPT_PP_CONST;

	scene &
	scene_node()
	FCPPT_PP_CONST;

	scene const &
	scene_node() const
	FCPPT_PP_CONST;

	overlay &
	overlay_node()
	FCPPT_PP_CONST;

	overlay const &
	overlay_node() const
	FCPPT_PP_CONST;

	fruitlib::scenic::delta::callback const
	ingame_clock_callback() const
	FCPPT_PP_CONST;

	fruitlib::scenic::delta::callback const
	standard_clock_callback() const
	FCPPT_PP_CONST;

	point_sprite::system_node &
	point_sprites()
	FCPPT_PP_CONST;

	point_sprite::system_node const &
	point_sprites() const
	FCPPT_PP_CONST;

	fruitapp::ingame_clock::float_type
	time_factor() const
	FCPPT_PP_CONST;

	void
	time_factor(
		fruitapp::ingame_clock::float_type);

	fruit::prototype_sequence const &
	fruit_prototypes() const
	FCPPT_PP_CONST;

	fruit::prototype_sequence &
	fruit_prototypes()
	FCPPT_PP_CONST;

	fruitapp::quick_log &
	quick_log()
	FCPPT_PP_CONST;

	fruitapp::viewport::manager &
	viewport_manager()
	FCPPT_PP_CONST;

	fruitapp::projection_manager::object &
	projection_manager()
	FCPPT_PP_CONST;

	~machine_impl();
private:
#ifdef FCPPT_CONFIG_WINDOWS_PLATFORM
	fruitlib::scoped_ostream_file_redirection error_redirection_;
	fruitlib::scoped_ostream_file_redirection log_redirection_;
	fruitlib::scoped_ostream_file_redirection output_redirection_;
	fruitlib::scoped_wostream_file_redirection werror_redirection_;
	fruitlib::scoped_wostream_file_redirection wlog_redirection_;
	fruitlib::scoped_wostream_file_redirection woutput_redirection_;
#endif
	fruitlib::random_generator random_generator_;
	sge::charconv::system_scoped_ptr const charconv_system_;
	sge::parse::json::object user_config_file_;
	sge::parse::json::object const config_file_;
	fruitapp::config_variables config_variables_;
	fruitapp::systems const systems_;
	sge::shader::context shader_context_;
	sge::model::md3::loader_scoped_ptr md3_loader_;
	fruitapp::viewport::manager viewport_manager_;
	fruitlib::font::cache font_cache_;
	fruitlib::log::scoped_sequence_ptr activated_loggers_;
	sge::timer::basic<sge::timer::clocks::standard> second_timer_;
	fruitapp::ingame_clock ingame_clock_;
	fruitlib::scenic::delta::duration ingame_clock_delta_;
	fruitlib::scenic::delta::duration standard_clock_delta_;
	fruitlib::audio::sound_controller sound_controller_;
	fcppt::signal::scoped_connection effects_volume_change_connection_;
	fruitlib::audio::music_controller music_controller_;
	fcppt::signal::scoped_connection music_volume_change_connection_;
	sge::camera::first_person::object camera_;
	fruitlib::scenic::adaptors::camera camera_node_;
	fcppt::signal::scoped_connection toggle_camera_connection_;
	fruitapp::projection_manager::object projection_manager_;
	fruitapp::renderable renderable_;
	fruitapp::gui::system_unique_ptr const gui_system_;
	fruitapp::quick_log quick_log_;
	fruitapp::directional_light_source main_light_source_;
	fruitapp::shadow_map shadow_map_;
	fruitapp::background background_;
	unsigned desired_fps_;
	fruitapp::highscore::score::value_type last_game_score_;
	fruitapp::point_sprite::system_node point_sprites_;
	fruitapp::screen_shooter screen_shooter_;
	fruitapp::fruit::prototype_sequence fruit_prototypes_;

	void
	toggle_camera();

	void
	manage_rendering();
};
}

#endif

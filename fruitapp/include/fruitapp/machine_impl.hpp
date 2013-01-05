#ifndef FRUITAPP_MACHINE_IMPL_HPP_INCLUDED
#define FRUITAPP_MACHINE_IMPL_HPP_INCLUDED

#include <fruitapp/config_variables/object.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/quick_log.hpp>
#include <fruitapp/renderable.hpp>
#include <fruitapp/screen_shooter.hpp>
#include <fruitapp/systems.hpp>
#include <fruitapp/background/node.hpp>
#include <fruitapp/cursor/manager.hpp>
#include <fruitapp/fruit/prototype_sequence.hpp>
#include <fruitapp/graphics_settings/object.hpp>
#include <fruitapp/gui/system_unique_ptr.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitapp/light/manager.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitapp/postprocessing/system_scoped_ptr.hpp>
#include <fruitapp/postprocessing/subsystems/main_fwd.hpp>
#include <fruitapp/projection_manager/object.hpp>
#include <fruitapp/shadow_map/object_scoped_ptr.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/basic_scoped_ostream_file_redirection_impl.hpp>
#include <fruitlib/random_generator.hpp>
#include <fruitlib/scoped_ostream_file_redirection.hpp>
#include <fruitlib/scoped_wostream_file_redirection.hpp>
#include <fruitlib/texture_manager.hpp>
#include <fruitlib/audio/music_controller.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/font/manager.hpp>
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
#include <sge/renderer/texture/emulate_srgb.hpp>
#include <sge/shader/optional_context_ref.hpp>
#include <sge/systems/instance.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <awl/main/exit_code.hpp>
#include <fcppt/config/platform.hpp>
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

	fruitlib::texture_manager &
	texture_manager()
	FCPPT_PP_CONST;

	fruitapp::graphics_settings::object &
	graphics_settings()
	FCPPT_PP_CONST;

	sge::shader::optional_context_ref const
	shader_context();

	sge::model::md3::loader &
	md3_loader()
	FCPPT_PP_CONST;

	fruitapp::config_variables::object &
	config_variables()
	FCPPT_PP_CONST;

	fruitapp::config_variables::object const &
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

	fruitapp::postprocessing::system &
	postprocessing_system()
	FCPPT_PP_CONST;

	fruitapp::postprocessing::subsystems::main &
	postprocessing_main()
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

	fruitapp::light::manager &
	light_manager()
	FCPPT_PP_CONST;

	fruitapp::shadow_map::optional_object_ref const
	shadow_map()
	FCPPT_PP_CONST;

	sge::camera::first_person::object &
	camera()
	FCPPT_PP_CONST;

	sge::camera::first_person::object const &
	camera() const
	FCPPT_PP_CONST;

	fruitlib::font::manager &
	font_manager()
	FCPPT_PP_CONST;

	fruitapp::cursor::manager &
	cursor_manager()
	FCPPT_PP_CONST;

	fruitapp::gui::system &
	gui_system();

	fruitlib::random_generator &
	random_generator()
	FCPPT_PP_CONST;

	fruitapp::highscore::score const
	last_game_score() const
	FCPPT_PP_CONST;

	void
	last_game_score(
		fruitapp::highscore::score const &);

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

	sge::renderer::texture::emulate_srgb
	emulate_srgb() const;

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
	fruitapp::config_variables::object config_variables_;
	fruitapp::graphics_settings::object graphics_settings_;
	sge::renderer::texture::emulate_srgb const emulate_srgb_;
	fruitapp::systems const systems_;
	fruitlib::texture_manager texture_manager_;
	fcppt::scoped_ptr<sge::shader::context> const shader_context_;
	sge::model::md3::loader_scoped_ptr md3_loader_;
	fruitapp::viewport::manager viewport_manager_;
	fruitapp::postprocessing::system_scoped_ptr const postprocessing_system_;
	fruitlib::font::manager font_manager_;
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
	fruitapp::light::manager light_manager_;
	fruitapp::shadow_map::object_scoped_ptr shadow_map_;
	fruitapp::background::node background_;
	unsigned desired_fps_;
	fruitapp::highscore::score last_game_score_;
	fruitapp::point_sprite::system_node point_sprites_;
	fruitapp::screen_shooter screen_shooter_;
	fruitapp::fruit::prototype_sequence fruit_prototypes_;
	fruitapp::cursor::manager cursor_manager_;

	void
	toggle_camera();

	void
	manage_rendering();
};
}

#endif

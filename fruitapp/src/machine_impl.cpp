#include <fruitapp/load_user_config.hpp>
#include <fruitapp/machine_impl.hpp>
#include <fruitapp/media_path.hpp>
#include <fruitapp/name.hpp>
#include <fruitapp/cursor/manager.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/depths/scene.hpp>
#include <fruitapp/gui/create_system.hpp>
#include <fruitapp/gui/system.hpp>
#include <fruitapp/postprocessing/create_system.hpp>
#include <fruitapp/postprocessing/system.hpp>
#include <fruitapp/shadow_map/object.hpp>
#include <fruitapp/shadow_map/object_unique_ptr.hpp>
#include <fruitlib/create_command_line_parameters.hpp>
#include <fruitlib/random_generator.hpp>
#include <fruitlib/scoped_frame_limiter.hpp>
#include <fruitlib/json/parse_projection.hpp>
#include <fruitlib/log/scoped_sequence_from_json.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/scenic/no_parent.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <sge/audio/loader_capabilities_field.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/camera/first_person/object.hpp>
#include <sge/camera/first_person/parameters.hpp>
#include <sge/charconv/utf8_file_to_fcppt_string_exn.hpp>
#include <sge/font/system.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/log/global_context.hpp>
#include <sge/media/extension.hpp>
#include <sge/media/extension_set.hpp>
#include <sge/media/optional_extension_set.hpp>
#include <sge/model/md3/create.hpp>
#include <sge/model/md3/loader.hpp>
#include <sge/model/md3/loader_fwd.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_file_exn.hpp>
#include <sge/parse/json/parse_string_exn.hpp>
#include <sge/parse/json/start.hpp>
#include <sge/parse/json/string_to_path.hpp>
#include <sge/parse/json/config/merge_command_line_parameters.hpp>
#include <sge/parse/json/config/merge_trees.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/parameters/object.hpp>
#include <sge/renderer/pixel_format/object.hpp>
#include <sge/renderer/target/onscreen.hpp>
#include <sge/shader/context.hpp>
#include <sge/sprite/parameters.hpp>
#include <sge/systems/audio_loader.hpp>
#include <sge/systems/audio_player_default.hpp>
#include <sge/systems/cursor_option_field.hpp>
#include <sge/systems/font.hpp>
#include <sge/systems/image2d.hpp>
#include <sge/systems/input.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/make_list.hpp>
#include <sge/systems/renderer.hpp>
#include <sge/systems/window.hpp>
#include <sge/timer/elapsed_and_reset.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <sge/viewport/fill_on_resize.hpp>
#include <sge/viewport/manager.hpp>
#include <sge/window/dim.hpp>
#include <sge/window/parameters.hpp>
#include <sge/window/system.hpp>
#include <sge/window/title.hpp>
#include <awl/main/exit_code.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/random/generator/seed_from_chrono.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/phoenix/core/reference.hpp>
#include <chrono>
#include <functional>
#include <memory>
#include <fcppt/config/external_end.hpp>


fruitapp::machine_impl::machine_impl(
	int const argc,
	char *argv[])
:
	node_base(
		fruitlib::scenic::no_parent()),
#ifdef FCPPT_CONFIG_WINDOWS_PLATFORM
	error_redirection_(
		std::cerr,
		"error.txt"),
	log_redirection_(
		std::clog,
		"log.txt"),
	output_redirection_(
		std::cout,
		"output.txt"),
	werror_redirection_(
		std::wcerr,
		"werror.txt"),
	wlog_redirection_(
		std::wclog,
		"wlog.txt"),
	woutput_redirection_(
		std::wcout,
		"woutput.txt"),
#endif
	random_generator_(
		fcppt::random::generator::seed_from_chrono<fruitlib::random_generator::seed>()),
	user_config_file_(
		fruitapp::load_user_config()),
	config_file_(
		sge::parse::json::config::merge_command_line_parameters(
			sge::parse::json::config::merge_trees(
				sge::parse::json::parse_string_exn(
					sge::charconv::utf8_file_to_fcppt_string_exn(
						fruitapp::media_path()/FCPPT_TEXT("config.json"))).object(),
				user_config_file_),
			fruitlib::create_command_line_parameters(
				argc,
				argv))),
	config_variables_(
		fruitapp::config_variables::global_config_ref(
			config_file_),
		fruitapp::config_variables::user_config_ref(
			user_config_file_)),
	graphics_settings_(
		config_file_,
		config_variables_.graphics_preset()),
	emulate_srgb_(
		sge::parse::json::find_and_convert_member<bool>(
			graphics_settings_.current(),
			sge::parse::json::string_to_path(
				FCPPT_TEXT("use-srgb")))
		?
			sge::renderer::texture::emulate_srgb::no
		:
			sge::renderer::texture::emulate_srgb::yes),
	systems_(
		sge::systems::make_list
			(sge::systems::renderer(
				sge::renderer::parameters::object(
					sge::renderer::pixel_format::object(
						sge::renderer::pixel_format::color::depth32,
						sge::renderer::pixel_format::depth_stencil::d24,
						sge::renderer::pixel_format::optional_multi_samples(),
						emulate_srgb_ == sge::renderer::texture::emulate_srgb::yes
						?
							sge::renderer::pixel_format::srgb::no
						:
							sge::renderer::pixel_format::srgb::yes),
					sge::renderer::parameters::vsync::on,
					sge::renderer::display_mode::optional_object()),
				sge::viewport::fill_on_resize()))
			(sge::systems::window(
				sge::window::parameters(
					sge::window::title(
						fruitapp::name()),
					sge::parse::json::find_and_convert_member<sge::window::dim>(
						config_file_,
						sge::parse::json::path(
							FCPPT_TEXT("window-size"))))))
			(sge::systems::font())
			(sge::systems::audio_player(
				sge::audio::player_capabilities_field::null()))
			(sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				sge::media::optional_extension_set(
					fcppt::assign::make_container<sge::media::extension_set>
						(sge::media::extension(FCPPT_TEXT("wav")))
						(sge::media::extension(FCPPT_TEXT("ogg"))))))
			(sge::systems::image2d(
				sge::image::capabilities_field::null(),
				sge::media::optional_extension_set(
					fcppt::assign::make_container<sge::media::extension_set>
						(sge::media::extension(FCPPT_TEXT("png"))))))
			(sge::systems::input(
				sge::systems::cursor_option_field::null()))),
	texture_manager_(
		systems_.renderer_core(),
		systems_.image_system(),
		emulate_srgb_),
	shader_context_(
		sge::parse::json::find_and_convert_member<bool>(
			graphics_settings_.current(),
			sge::parse::json::path(
				FCPPT_TEXT("use-shaders")))
		?
			fcppt::make_unique_ptr<sge::shader::context>(
				systems_.renderer_core())
		:
			std::unique_ptr<sge::shader::context>()),
	md3_loader_(
		sge::model::md3::create()),
	viewport_manager_(
		systems_.viewport_manager(),
		systems_.renderer_core().onscreen_target()),
	postprocessing_system_(
		fruitapp::postprocessing::create_system(
			this->shader_context(),
			viewport_manager_,
			graphics_settings_,
			config_file_)),
	font_manager_(
		systems_.renderer_ffp(),
		emulate_srgb_,
		systems_.font_system(),
		texture_manager_,
		sge::parse::json::parse_file_exn(
			fruitapp::media_path() / FCPPT_TEXT("fonts.json")).object(),
		fruitlib::font::base_path(
			fruitapp::media_path())),
	activated_loggers_(
		fruitlib::log::scoped_sequence_from_json(
			sge::log::global_context(),
			sge::parse::json::find_and_convert_member<sge::parse::json::array const>(
				config_file_,
				sge::parse::json::path(
					FCPPT_TEXT("loggers"))
					/ FCPPT_TEXT("sge")))),
	second_timer_(
		sge::timer::parameters<sge::timer::clocks::standard>(
			std::chrono::seconds(
				1))),
	ingame_clock_(),
	ingame_clock_delta_(),
	standard_clock_delta_(),
	sound_controller_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		random_generator_,
		fruitapp::media_path()/FCPPT_TEXT("sounds"),
		systems_.audio_loader(),
		systems_.audio_player(),
		sge::audio::scalar(
			sge::parse::json::find_and_convert_member<sge::audio::scalar>(
				config_file(),
				sge::parse::json::path(
					FCPPT_TEXT("effects-volume"))))),
	effects_volume_change_connection_(
		config_variables_.effects_volume().change_callback(
			std::bind(
				static_cast<void(fruitlib::audio::sound_controller::*)(sge::audio::scalar)>(
					&fruitlib::audio::sound_controller::gain),
				&sound_controller_,
				std::placeholders::_1))),
	music_controller_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		standard_clock_callback(),
		random_generator_,
		systems_.audio_loader(),
		systems_.audio_player(),
		fruitlib::time_format::find_and_convert_duration<fruitlib::scenic::delta::duration>(
			config_file(),
			sge::parse::json::path(
				FCPPT_TEXT("music"))
				/ FCPPT_TEXT("crossfade-time")),
		fruitapp::media_path()/FCPPT_TEXT("music"),
		sge::parse::json::find_and_convert_member<sge::audio::scalar>(
			config_file(),
			sge::parse::json::path(
				FCPPT_TEXT("music"))
				/ FCPPT_TEXT("volume"))),
	music_volume_change_connection_(
		config_variables_.music_volume().change_callback(
			std::bind(
				static_cast<void(fruitlib::audio::music_controller::*)(sge::audio::scalar)>(
					&fruitlib::audio::music_controller::gain),
				&music_controller_,
				std::placeholders::_1))),
	camera_(
		sge::camera::first_person::parameters(
			systems_.keyboard_collector(),
			systems_.mouse_collector(),
			sge::camera::is_active(
				false),
			sge::camera::first_person::movement_speed(
				sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
					config_file_,
					sge::parse::json::path(FCPPT_TEXT("ingame"))
						/ FCPPT_TEXT("camera")
						/ FCPPT_TEXT("movement-speed"))),
			sge::camera::coordinate_system::object(
				sge::camera::coordinate_system::right(
					sge::renderer::vector3(
						1.0f,
						0.0f,
						0.0f)),
				sge::camera::coordinate_system::up(
					sge::renderer::vector3(
						0.0f,
						1.0f,
						0.0f)),
				sge::camera::coordinate_system::forward(
					sge::renderer::vector3(
						0.0f,
						0.0f,
						1.0f)),
				sge::camera::coordinate_system::position(
					-sge::parse::json::find_and_convert_member<sge::renderer::vector3>(
						config_file_,
						sge::parse::json::path(FCPPT_TEXT("ingame"))
							/ FCPPT_TEXT("camera")
							/ FCPPT_TEXT("initial-position")))))),
	camera_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		camera_,
		standard_clock_callback()),
	toggle_camera_connection_(
		systems_.keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f2,
				std::bind(
					&machine_impl::toggle_camera,
					this)))),
	projection_manager_(
		sge::parse::json::find_and_convert_member<sge::parse::json::object const>(
			config_file_,
			sge::parse::json::path(FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("camera")
				/ FCPPT_TEXT("projection")),
		viewport_manager_,
		camera_),
	renderable_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				this->root_node(),
				fruitlib::scenic::depth(
					depths::root::scene))),
		systems_.renderer_ffp(),
		*postprocessing_system_),
	gui_system_(
		fruitapp::gui::create_system(
			fruitlib::scenic::parent(
				this->overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::dont_care)),
			systems_.renderer_ffp(),
			systems_.image_system(),
			systems_.viewport_manager(),
			this->standard_clock_callback(),
			systems_.keyboard_collector(),
			systems_.cursor_demuxer(),
			this->sound_controller(),
			this->emulate_srgb())),
	quick_log_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		sge::parse::json::find_and_convert_member<sge::parse::json::object const>(
			config_file_,
			sge::parse::json::path(
				FCPPT_TEXT("quick-log"))),
		font_manager_,
		viewport_manager_,
		sound_controller_),
	light_manager_(
		sge::parse::json::parse_file_exn(
			fruitapp::media_path() / FCPPT_TEXT("light.json")).object()),
	shadow_map_(
		this->shader_context().has_value() &&
		sge::parse::json::find_and_convert_member<bool>(
			graphics_settings_.current(),
			sge::parse::json::path(
				FCPPT_TEXT("use-shadow-map")))
		?
			fcppt::make_unique_ptr<fruitapp::shadow_map::object>(
				fruitlib::scenic::optional_parent(
					fruitlib::scenic::parent(
						this->root_node(),
						fruitlib::scenic::depth(
							depths::root::shadow_map))),
				sge::parse::json::find_and_convert_member<sge::parse::json::object const>(
					config_file_,
					sge::parse::json::path(
						FCPPT_TEXT("shadow-map"))),
				systems_.renderer_ffp(),
				light_manager_)
		:
			fruitapp::shadow_map::object_unique_ptr()),
	background_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				scene_node(),
				fruitlib::scenic::depth(
					depths::scene::background))),
		this->texture_manager(),
		systems_.renderer_core(),
		fruitapp::background::use_ffp(
			sge::parse::json::find_and_convert_member<bool>(
				config_file_,
				sge::parse::json::string_to_path(
					fcppt::string(
						FCPPT_TEXT("background/use-ffp"))))),
		fruitapp::background::repetitions(
			sge::parse::json::find_and_convert_member<fruitapp::background::repetitions::value_type>(
				config_file_,
				sge::parse::json::string_to_path(
					fcppt::string(
						FCPPT_TEXT("background/repetitions"))))),
		camera_,
		this->projection_manager(),
		sge::shader::optional_context_ref(
			this->shader_context()),
		this->shadow_map()),
	desired_fps_(
		sge::parse::json::find_and_convert_member<unsigned>(
			config_file(),
			sge::parse::json::path(FCPPT_TEXT("desired-fps")))),
	last_game_score_(
		// Something invalid so you get the error (if there is one)
		31337u),
	point_sprites_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				scene_node(),
				fruitlib::scenic::depth(
					depths::scene::splatter))),
		fruitapp::media_path()/FCPPT_TEXT("point_sprites"),
		random_generator_,
		systems_.renderer_ffp(),
		projection_manager_,
		texture_manager_),
	screen_shooter_(
		systems_.keyboard_collector(),
		systems_.renderer_core(),
		systems_.image_system(),
		quick_log_),
	fruit_prototypes_(),
	cursor_manager_(
		systems_.input_processor(),
		systems_.renderer_ffp(),
		systems_.renderer_ffp().onscreen_target(),
		camera_,
		texture_manager_,
		viewport_manager_,
		sound_controller_,
		ingame_clock_,
		sge::parse::json::parse_file_exn(
			fruitapp::media_path() / FCPPT_TEXT("cursor.json")).object())
{
	systems_.audio_player().gain(
		sge::parse::json::find_and_convert_member<sge::audio::scalar>(
			config_file(),
			sge::parse::json::path(
				FCPPT_TEXT("audio-volume"))));
}

sge::parse::json::object const &
fruitapp::machine_impl::config_file() const
{
	return config_file_;
}

fruitapp::systems const &
fruitapp::machine_impl::systems() const
{
	return systems_;
}

fruitlib::texture_manager &
fruitapp::machine_impl::texture_manager()
{
	return
		texture_manager_;
}

fruitapp::graphics_settings::object &
fruitapp::machine_impl::graphics_settings()
{
	return
		graphics_settings_;
}

sge::shader::optional_context_ref const
fruitapp::machine_impl::shader_context()
{
	return
		shader_context_
		?
			sge::shader::optional_context_ref(
				*shader_context_)
		:
			sge::shader::optional_context_ref();
}

sge::model::md3::loader &
fruitapp::machine_impl::md3_loader()
{
	return *md3_loader_;
}

fruitapp::config_variables::object &
fruitapp::machine_impl::config_variables()
{
	return config_variables_;
}

fruitapp::config_variables::object const &
fruitapp::machine_impl::config_variables() const
{
	return config_variables_;
}

bool
fruitapp::machine_impl::run_once()
{
	if(!systems_.window_system().poll())
		return false;

	fruitlib::scoped_frame_limiter sfl(
		static_cast<fruitlib::scoped_frame_limiter::fps_type>(
			desired_fps_));

	standard_clock_delta_ =
		sge::timer::elapsed_and_reset<fruitlib::scenic::delta::duration>(
			second_timer_);

	ingame_clock_delta_ =
		fruitapp::ingame_clock::duration(
			static_cast<fruitapp::ingame_clock::duration::rep>(
				ingame_clock_.factor() *
				static_cast<fruitapp::ingame_clock::float_type>(
					std::chrono::duration_cast<fruitapp::ingame_clock::duration>(
						standard_clock_delta_).count())));

	ingame_clock_.update();

	fruitlib::scenic::events::update event;

	node_base::forward_to_children(
		event);

	return true;
}

void
fruitapp::machine_impl::quit(
	awl::main::exit_code const _exit_code)
{
	systems_.window_system().quit(
		_exit_code);
}

awl::main::exit_code const
fruitapp::machine_impl::exit_code() const
{
	return
		systems_.window_system().exit_code();
}

fruitapp::postprocessing::system &
fruitapp::machine_impl::postprocessing_system()
{
	return
		*postprocessing_system_;
}

fruitapp::postprocessing::subsystems::main &
fruitapp::machine_impl::postprocessing_main()
{
	return
		renderable_.postprocessing_main();
}

fruitapp::ingame_clock const &
fruitapp::machine_impl::ingame_clock() const
{
	return
		ingame_clock_;
}

fruitlib::audio::sound_controller &
fruitapp::machine_impl::sound_controller()
{
	return sound_controller_;
}

fruitlib::audio::sound_controller const &
fruitapp::machine_impl::sound_controller() const
{
	return sound_controller_;
}

fruitlib::audio::music_controller &
fruitapp::machine_impl::music_controller()
{
	return music_controller_;
}

fruitlib::audio::music_controller const &
fruitapp::machine_impl::music_controller() const
{
	return music_controller_;
}

fruitapp::light::manager &
fruitapp::machine_impl::light_manager()
{
	return light_manager_;
}

fruitapp::shadow_map::optional_object_ref const
fruitapp::machine_impl::shadow_map()
{
	return
		shadow_map_
		?
			fruitapp::shadow_map::optional_object_ref(
				*shadow_map_)
		:
			fruitapp::shadow_map::optional_object_ref();
}

sge::camera::first_person::object &
fruitapp::machine_impl::camera()
{
	return camera_;
}

sge::camera::first_person::object const &
fruitapp::machine_impl::camera() const
{
	return camera_;
}

fruitlib::font::manager &
fruitapp::machine_impl::font_manager()
{
	return font_manager_;
}

fruitapp::cursor::manager &
fruitapp::machine_impl::cursor_manager()
{
	return cursor_manager_;
}


fruitapp::gui::system &
fruitapp::machine_impl::gui_system()
{
	return
		*gui_system_;
}

fruitlib::random_generator &
fruitapp::machine_impl::random_generator()
{
	return random_generator_;
}

fruitapp::highscore::score const
fruitapp::machine_impl::last_game_score() const
{
	return last_game_score_;
}

void
fruitapp::machine_impl::last_game_score(
	fruitapp::highscore::score const &_last_game_score)
{
	last_game_score_ = _last_game_score;
}

fruitlib::scenic::base &
fruitapp::machine_impl::root_node()
{
	return *this;
}

fruitlib::scenic::base const &
fruitapp::machine_impl::root_node() const
{
	return *this;
}

fruitapp::scene &
fruitapp::machine_impl::scene_node()
{
	return renderable_.scene();
}

fruitapp::scene const &
fruitapp::machine_impl::scene_node() const
{
	return renderable_.scene();
}

fruitapp::overlay &
fruitapp::machine_impl::overlay_node()
{
	return renderable_.overlay();
}

fruitapp::overlay const &
fruitapp::machine_impl::overlay_node() const
{
	return renderable_.overlay();
}

fruitlib::scenic::delta::callback const
fruitapp::machine_impl::ingame_clock_callback() const
{
	return
		boost::phoenix::ref(
			ingame_clock_delta_);
}

fruitlib::scenic::delta::callback const
fruitapp::machine_impl::standard_clock_callback() const
{
	return
		boost::phoenix::ref(
			standard_clock_delta_);
}

fruitapp::point_sprite::system_node &
fruitapp::machine_impl::point_sprites()
{
	return point_sprites_;
}

fruitapp::point_sprite::system_node const &
fruitapp::machine_impl::point_sprites() const
{
	return point_sprites_;
}

fruitapp::ingame_clock::float_type
fruitapp::machine_impl::time_factor() const
{
	return ingame_clock_.factor();
}

sge::renderer::texture::emulate_srgb
fruitapp::machine_impl::emulate_srgb() const
{
	return
		emulate_srgb_;
}

void
fruitapp::machine_impl::time_factor(
 fruitapp::ingame_clock::float_type const _time_factor)
{
	ingame_clock_.factor(
		_time_factor);
}

fruitapp::fruit::prototype_sequence const &
fruitapp::machine_impl::fruit_prototypes() const
{
	return fruit_prototypes_;
}

fruitapp::fruit::prototype_sequence &
fruitapp::machine_impl::fruit_prototypes()
{
	return fruit_prototypes_;
}

fruitapp::quick_log &
fruitapp::machine_impl::quick_log()
{
	return quick_log_;
}

fruitapp::viewport::manager &
fruitapp::machine_impl::viewport_manager()
{
	return viewport_manager_;
}

fruitapp::projection_manager::object &
fruitapp::machine_impl::projection_manager()
{
	return projection_manager_;
}

fruitapp::machine_impl::~machine_impl()
{
}

void
fruitapp::machine_impl::toggle_camera()
{
	camera_.toggle_is_active();
}


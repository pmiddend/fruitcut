#include <fruitapp/light_source_from_json.hpp>
#include <fruitapp/load_user_config.hpp>
#include <fruitapp/machine_impl.hpp>
#include <fruitapp/name.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/depths/scene.hpp>
#include <fruitlib/create_command_line_parameters.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/random_generator.hpp>
#include <fruitlib/scoped_frame_limiter.hpp>
#include <fruitlib/utf8_file_to_fcppt_string_exn.hpp>
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
#include <sge/cegui/cursor_visibility.hpp>
#include <sge/cegui/load_context.hpp>
#include <sge/cegui/syringe.hpp>
#include <sge/cegui/system.hpp>
#include <sge/charconv/create_system.hpp>
#include <sge/charconv/system.hpp>
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
#include <sge/parse/json/parse_string_exn.hpp>
#include <sge/parse/json/config/merge_command_line_parameters.hpp>
#include <sge/parse/json/config/merge_trees.hpp>
#include <sge/sprite/parameters.hpp>
#include <sge/systems/audio_loader.hpp>
#include <sge/systems/make_list.hpp>
#include <sge/renderer/parameters/object.hpp>
#include <sge/renderer/pixel_format/object.hpp>
#include <sge/systems/audio_player_default.hpp>
#include <sge/systems/charconv.hpp>
#include <sge/systems/cursor_option_field.hpp>
#include <sge/systems/font.hpp>
#include <sge/systems/image2d.hpp>
#include <sge/systems/input.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
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
#include <fcppt/nonassignable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/random/generator/seed_from_chrono.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/spirit/home/phoenix/core.hpp>
#include <fcppt/config/external_end.hpp>


namespace
{
struct phoenix_ref_broken_hack
{
FCPPT_NONASSIGNABLE(
	phoenix_ref_broken_hack);
public:
	explicit
	phoenix_ref_broken_hack(
		fruitlib::scenic::delta::duration const &_duration)
	:
		duration_(
			_duration)
	{
	}

	fruitlib::scenic::delta::duration const
	operator()() const
	{
		return duration_;
	}

	fruitlib::scenic::delta::duration const &duration_;
};
}

fruitapp::machine_impl::machine_impl(
	int const argc,
	char *argv[])
:
	node_base(
		fruitlib::scenic::no_parent()),
	random_generator_(
		fcppt::random::generator::seed_from_chrono<fruitlib::random_generator::seed>()),
	charconv_system_(
		sge::charconv::create_system()),
	user_config_file_(
		fruitapp::load_user_config(
			*charconv_system_)),
	config_file_(
		sge::parse::json::config::merge_command_line_parameters(
			sge::parse::json::config::merge_trees(
				sge::parse::json::parse_string_exn(
					fruitlib::utf8_file_to_fcppt_string_exn(
						*charconv_system_,
						fruitlib::media_path()/FCPPT_TEXT("config.json"))).object(),
				user_config_file_),
			fruitlib::create_command_line_parameters(
				argc,
				argv))),
	config_variables_(
		config_file_,
		user_config_file_),
	systems_(
		sge::systems::make_list
			(sge::systems::renderer(
				sge::renderer::parameters::object(
					sge::renderer::pixel_format::object(
						sge::renderer::pixel_format::color::depth32,
						sge::renderer::pixel_format::depth_stencil::d24,
						sge::renderer::pixel_format::optional_multi_samples(),
						sge::renderer::pixel_format::srgb::no),
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
							FCPPT_TEXT("graphics"))
							/ FCPPT_TEXT("window-size")))))
			(sge::systems::charconv(
				*charconv_system_))
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
	shader_context_(
		systems_.renderer_core()),
	md3_loader_(
		sge::model::md3::create()),
	viewport_manager_(
		systems_.viewport_manager()),
	renderable_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				root_node(),
				fruitlib::scenic::depth(
					depths::root::scene))),
		this->shader_context(),
		config_file_,
		viewport_manager_),
	activated_loggers_(
		fruitlib::log::scoped_sequence_from_json(
			sge::log::global_context(),
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file_,
				sge::parse::json::path(
					FCPPT_TEXT("loggers"))
					/ FCPPT_TEXT("sge")))),
	font_cache_(
		systems_.font_system(),
		systems_.image_system(),
		sge::parse::json::find_and_convert_member<sge::parse::json::object>(
			config_file_,
			sge::parse::json::path(
				FCPPT_TEXT("fonts"))),
		fruitlib::font::base_path(
			fruitlib::media_path())),
	second_timer_(
		sge::timer::parameters<sge::timer::clocks::standard>(
			boost::chrono::seconds(
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
		fruitlib::media_path()/FCPPT_TEXT("sounds"),
		systems_.audio_loader(),
		systems_.audio_player(),
		sge::audio::scalar(
			sge::parse::json::find_and_convert_member<sge::audio::scalar>(
				config_file(),
				sge::parse::json::path(
					FCPPT_TEXT("effects-volume"))))),
	effects_volume_change_connection_(
		config_variables_.effects_volume().change_callback(
			std::tr1::bind(
				static_cast<void(fruitlib::audio::sound_controller::*)(sge::audio::scalar)>(
					&fruitlib::audio::sound_controller::gain),
				&sound_controller_,
				std::tr1::placeholders::_1))),
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
		fruitlib::media_path()/FCPPT_TEXT("music"),
		sge::parse::json::find_and_convert_member<sge::audio::scalar>(
			config_file(),
			sge::parse::json::path(
				FCPPT_TEXT("music"))
				/ FCPPT_TEXT("volume"))),
	music_volume_change_connection_(
		config_variables_.music_volume().change_callback(
			std::tr1::bind(
				static_cast<void(fruitlib::audio::music_controller::*)(sge::audio::scalar)>(
					&fruitlib::audio::music_controller::gain),
				&music_controller_,
				std::tr1::placeholders::_1))),
	quick_log_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		this->systems().renderer_ffp(),
		config_file_,
		font_cache_,
		viewport_manager_,
		sound_controller_),
	camera_(
		sge::camera::first_person::parameters(
			systems_.keyboard_collector(),
			systems_.mouse_collector(),
			sge::camera::first_person::is_active(
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
	projection_manager_(
		sge::parse::json::find_and_convert_member<sge::parse::json::object>(
			config_file_,
			sge::parse::json::path(FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("camera")
				/ FCPPT_TEXT("projection")),
		viewport_manager_,
		camera_),
	toggle_camera_connection_(
		systems_.keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f2,
				std::tr1::bind(
					&machine_impl::toggle_camera,
					this)))),
	main_light_source_(
		fruitapp::light_source_from_json(
			sge::parse::json::find_and_convert_member<sge::parse::json::object>(
				config_file_,
				sge::parse::json::path(
					FCPPT_TEXT("main-light-source"))))),
	shadow_map_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				root_node(),
				fruitlib::scenic::depth(
					depths::root::shadow_map))),
		config_file_,
		systems_.renderer_ffp(),
		main_light_source_.model_view()),
	background_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				scene_node(),
				fruitlib::scenic::depth(
					depths::scene::background))),
		systems_.image_system(),
		this->shader_context(),
		shadow_map_.mvp(),
		shadow_map_.texture(),
		config_file_,
		camera_,
		this->projection_manager()),
	desired_fps_(
		sge::parse::json::find_and_convert_member<unsigned>(
			config_file(),
			sge::parse::json::path(FCPPT_TEXT("graphics"))
				/ FCPPT_TEXT("desired-fps"))),
	gui_system_(
		sge::cegui::load_context(
			fruitlib::media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("fruitcut.scheme"))
			.font_directory(
				fruitlib::media_path()/FCPPT_TEXT("fonts")),
		systems_.renderer_ffp(),
		systems_.image_system(),
		systems_.charconv_system(),
		systems_.viewport_manager(),
		sge::cegui::cursor_visibility::invisible),
	gui_syringe_(
		gui_system_),
	last_game_score_(
		// Something invalid so you get the error (if there is one)
		31337),
	point_sprites_(
		/*
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				scene_node(),
				fruitlib::scenic::depth(
					depths::scene::splatter))),
		fruitlib::media_path()/FCPPT_TEXT("point_sprites"),
		random_generator_,
		systems_.renderer(),
		systems_.image_system(),
		camera_*/),
	screen_shooter_(
		systems_.keyboard_collector(),
		systems_.renderer_core(),
		systems_.image_system(),
		quick_log_),
	fruit_prototypes_()
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

sge::shader::context &
fruitapp::machine_impl::shader_context()
{
	return
		shader_context_;
}

sge::model::md3::loader &
fruitapp::machine_impl::md3_loader()
{
	return *md3_loader_;
}

fruitapp::config_variables &
fruitapp::machine_impl::config_variables()
{
	return config_variables_;
}

fruitapp::config_variables const &
fruitapp::machine_impl::config_variables() const
{
	return config_variables_;
}

fruitapp::postprocessing &
fruitapp::machine_impl::postprocessing()
{
	return renderable_.postprocessing();
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
					boost::chrono::duration_cast<fruitapp::ingame_clock::duration>(
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

fruitapp::background &
fruitapp::machine_impl::background()
{
	return background_;
}

fruitapp::background const &
fruitapp::machine_impl::background() const
{
	return background_;
}

fruitapp::directional_light_source const &
fruitapp::machine_impl::main_light_source()
{
	return main_light_source_;
}

fruitapp::shadow_map &
fruitapp::machine_impl::shadow_map()
{
	return shadow_map_;
}

fruitapp::shadow_map const &
fruitapp::machine_impl::shadow_map() const
{
	return shadow_map_;
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

fruitlib::font::cache &
fruitapp::machine_impl::font_cache()
{
	return font_cache_;
}

fruitlib::font::cache const &
fruitapp::machine_impl::font_cache() const
{
	return font_cache_;
}

sge::cegui::system &
fruitapp::machine_impl::gui_system()
{
	return gui_system_;
}

sge::cegui::system const &
fruitapp::machine_impl::gui_system() const
{
	return gui_system_;
}

sge::cegui::syringe &
fruitapp::machine_impl::gui_syringe()
{
	return gui_syringe_;
}

sge::cegui::syringe const &
fruitapp::machine_impl::gui_syringe() const
{
	return gui_syringe_;
}

fruitlib::random_generator &
fruitapp::machine_impl::random_generator()
{
	return random_generator_;
}

fruitapp::highscore::score::value_type
fruitapp::machine_impl::last_game_score() const
{
	return last_game_score_;
}

void
fruitapp::machine_impl::last_game_score(
	highscore::score::value_type const &_last_game_score)
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
		phoenix_ref_broken_hack(
			ingame_clock_delta_);
}

fruitlib::scenic::delta::callback const
fruitapp::machine_impl::standard_clock_callback() const
{
	return
		phoenix_ref_broken_hack(
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


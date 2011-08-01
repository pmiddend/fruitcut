#include <fruitapp/machine_impl.hpp>
#include <fruitapp/name.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/depths/scene.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitlib/json/parse_projection.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <fruitlib/json/merge_trees.hpp>
#include <fruitlib/json/parse_string_exn.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <fruitlib/json/merge_command_line_parameters.hpp>
#include <fruitlib/utf8_file_to_fcppt_string.hpp>
#include <fruitlib/create_command_line_parameters.hpp>
#include <fruitlib/log/scoped_sequence_from_json.hpp>
#include <fruitlib/random_generator.hpp>
#include <fruitlib/scenic/no_parent.hpp>
#include <fruitlib/scenic/update_duration.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/time_format/string_to_duration.hpp>
#include <fruitapp/light_source_from_json.hpp>
#include <fruitapp/load_user_config.hpp>
#include <media_path.hpp>
#include <sge/audio/player.hpp>
#include <sge/camera/camera.hpp>
#include <sge/cegui/cursor_visibility.hpp>
#include <sge/cegui/load_context.hpp>
#include <sge/cegui/syringe.hpp>
#include <sge/cegui/system.hpp>
#include <sge/extension_set.hpp>
#include <sge/font/size_type.hpp>
#include <sge/font/system.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/log/global_context.hpp>
#include <sge/model/md3/create.hpp>
#include <sge/parse/json/json.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/depth_stencil_buffer.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <sge/renderer/visual_depth.hpp>
#include <sge/renderer/vsync.hpp>
#include <sge/sprite/parameters.hpp>
#include <sge/systems/systems.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/time/time.hpp>
#include <sge/viewport/fill_on_resize.hpp>
#include <sge/viewport/manager.hpp>
#include <sge/window/instance.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/chrono/high_resolution_clock.hpp>
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/chrono/duration_impl.hpp>
#include <fcppt/chrono/duration_cast.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/time/sleep_any.hpp>
#include <fcppt/tr1/functional.hpp>

fruitapp::machine_impl::machine_impl(
	int const argc,
	char *argv[])
:
	node_base(
		fruitlib::scenic::no_parent()),
	random_generator_(
		static_cast<fruitlib::random_generator::result_type>(
			fcppt::chrono::high_resolution_clock::now().time_since_epoch().count())),
	user_config_file_(
		fruitapp::load_user_config()),
	config_file_(
		fruitlib::json::merge_command_line_parameters(
			fruitlib::json::merge_trees(
				fruitlib::json::parse_string_exn(
					fruitlib::utf8_file_to_fcppt_string(
						fruitcut::media_path()/FCPPT_TEXT("config.json"))),
				user_config_file_),
			fruitlib::create_command_line_parameters(
				argc,
				argv))),
	config_variables_(
		config_file_,
		user_config_file_),
	systems_(
		sge::systems::list()
			(sge::systems::window(
				sge::window::simple_parameters(
					fruitapp::name(),
					fruitlib::json::find_and_convert_member<sge::window::dim>(
						config_file_,
						fruitlib::json::path(
							FCPPT_TEXT("graphics"))
							/ FCPPT_TEXT("window-size")))))
			(sge::systems::renderer(
				sge::renderer::parameters(
					sge::renderer::visual_depth::depth32,
					sge::renderer::depth_stencil_buffer::d24,
					sge::renderer::vsync::on,
					sge::renderer::no_multi_sampling),
				sge::viewport::fill_on_resize()))
			(sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector) 
					| sge::systems::input_helper::mouse_collector
					| sge::systems::input_helper::cursor_demuxer,
					sge::systems::cursor_option_field())) 
			(sge::systems::audio_player_default())
			(sge::systems::parameterless::charconv)
			(sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>
					(FCPPT_TEXT("wav"))
					(FCPPT_TEXT("ogg"))))
			(sge::systems::parameterless::font) 	
			(sge::systems::image_loader(
					sge::image::capabilities_field::null(),
					fcppt::assign::make_container<sge::extension_set>
						(FCPPT_TEXT("png"))))),
	md3_loader_(
		sge::model::md3::create()),
	renderable_(
		fruitlib::scenic::parent(
			root_node(),
			fruitlib::scenic::depth(
				depths::root::scene)),
		systems_,
		config_file_),
	activated_loggers_(
		fruitlib::log::scoped_sequence_from_json(
			sge::log::global_context(),
			fruitlib::json::find_and_convert_member<sge::parse::json::array>(
				config_file_,
				fruitlib::json::path(
					FCPPT_TEXT("loggers"))
					/ FCPPT_TEXT("sge")))),
	font_cache_(
		systems_.font_system(),
		systems_.renderer(),
		systems_.image_loader(),
		fruitcut::media_path(),
		fruitlib::json::find_and_convert_member<sge::parse::json::object>(
			config_file_,
			fruitlib::json::path(
				FCPPT_TEXT("fonts")))),
	second_timer_(
		sge::time::second(
			1)),
	current_time_(
		sge::time::clock::now()),
	transformed_time_(
		current_time_),
	time_factor_(
		static_cast<sge::time::funit>(
			1)),
	sound_controller_(
		fruitlib::scenic::parent(
			root_node(),
			fruitlib::scenic::depth(
				depths::root::dont_care)),
		random_generator_,
		fruitcut::media_path()/FCPPT_TEXT("sounds"),
		systems_.audio_loader(),
		systems_.audio_player(),
		sge::audio::scalar(
			fruitlib::json::find_and_convert_member<sge::audio::scalar>(
				config_file(),
				fruitlib::json::path(
					FCPPT_TEXT("effects-volume"))))),
	effects_volume_change_connection_(
		config_variables_.effects_volume().change_callback(
			std::tr1::bind(
				static_cast<void(fruitlib::audio::sound_controller::*)(sge::audio::scalar)>(
					&fruitlib::audio::sound_controller::gain),
				&sound_controller_,
				std::tr1::placeholders::_1))),
	music_controller_(
		fruitlib::scenic::parent(
			root_node(),
			fruitlib::scenic::depth(
				depths::root::dont_care)),
		random_generator_,
		systems_.audio_loader(),
		systems_.audio_player(),
		*fruitlib::time_format::string_to_duration<sge::time::duration>(
			fruitlib::json::find_and_convert_member<fcppt::string>(
				config_file(),
				fruitlib::json::path(
					FCPPT_TEXT("music"))
					/ FCPPT_TEXT("crossfade-time"))),
		fruitcut::media_path()/FCPPT_TEXT("music"),
		fruitlib::json::find_and_convert_member<sge::audio::scalar>(
			config_file(),
			fruitlib::json::path(
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
		fruitlib::scenic::parent(
			overlay_node(),
			fruitlib::scenic::depth(
				depths::overlay::dont_care)),
		config_file_,
		font_cache_,
		systems_.renderer(),
		sound_controller_),
	camera_(
		sge::camera::parameters(
			sge::camera::movement_speed(
				fruitlib::json::find_and_convert_member<sge::renderer::scalar>(
					config_file_,
					fruitlib::json::path(FCPPT_TEXT("ingame"))
						/ FCPPT_TEXT("camera")
						/ FCPPT_TEXT("movement-speed"))),
			// mousespeed
			sge::camera::rotation_speed(
				fruitlib::json::find_and_convert_member<sge::renderer::scalar>(
					config_file_,
					fruitlib::json::path(FCPPT_TEXT("ingame"))
						/ FCPPT_TEXT("camera")
						/ FCPPT_TEXT("mouse-speed"))),
			systems_.keyboard_collector(),
			systems_.mouse_collector())
			.active(
				false)
			.gizmo(
				sge::camera::identity_gizmo()
					.position(
						fruitlib::json::find_and_convert_member<sge::renderer::vector3>(
							config_file_,
							fruitlib::json::path(FCPPT_TEXT("ingame"))
								/ FCPPT_TEXT("camera")
								/ FCPPT_TEXT("initial-position"))))), 
	camera_node_(
		fruitlib::scenic::parent(
			root_node(),
			fruitlib::scenic::depth(
				depths::root::dont_care)),	
		camera_),
	toggle_camera_connection_(
		systems_.keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f2, 
				std::tr1::bind(
					&machine_impl::toggle_camera,
					this)))),
	viewport_change_connection_(
		systems_.viewport_manager().manage_callback(
			std::tr1::bind(
				&machine_impl::viewport_change,
				this))),
	main_light_source_(
		fruitapp::light_source_from_json(
			fruitlib::json::find_and_convert_member<sge::parse::json::object>(
				config_file_,
				fruitlib::json::path(
					FCPPT_TEXT("main-light-source"))))),
	shadow_map_(
		fruitlib::scenic::parent(
			root_node(),
			fruitlib::scenic::depth(
				depths::root::shadow_map)),
		config_file_,
		systems_.renderer(),
		main_light_source_.model_view()),
	background_(
		fruitlib::scenic::parent(
			scene_node(),
			fruitlib::scenic::depth(
				depths::scene::background)),
		systems_.renderer(),
		systems_.image_loader(),
		shadow_map_.texture(),
		shadow_map_.mvp(),
		config_file_,
		camera_),
	desired_fps_(
		fruitlib::json::find_and_convert_member<fcppt::chrono::milliseconds::rep>(
			config_file(),
			fruitlib::json::path(FCPPT_TEXT("graphics"))
				/ FCPPT_TEXT("desired-fps"))),
	gui_system_(
		sge::cegui::load_context(
			fruitcut::media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("fruitcut.scheme"))
			.font_directory(
				fruitcut::media_path()/FCPPT_TEXT("fonts")),
		systems_.renderer(),
		systems_.image_loader(),
		systems_.charconv_system(),
		systems_.viewport_manager(),
		sge::cegui::cursor_visibility::invisible),
	gui_syringe_(
		gui_system_),
	last_game_score_(
		// Something invalid so you get the error (if there is one)
		31337),
	point_sprites_(
		fruitlib::scenic::parent(
			scene_node(),
			fruitlib::scenic::depth(
				depths::scene::splatter)),
		fruitcut::media_path()/FCPPT_TEXT("point_sprites"),
		random_generator_,
		systems_.renderer(),
		systems_.image_loader(),
		camera_),
	screen_shooter_(
		systems_.keyboard_collector(),
		systems_.renderer(),
		systems_.image_loader(),
		quick_log_),
	fruit_prototypes_()
{
	systems_.audio_player().gain(
		fruitlib::json::find_and_convert_member<sge::audio::scalar>(
			config_file(),
			fruitlib::json::path(
				FCPPT_TEXT("audio-volume"))));
}

sge::parse::json::object const &
fruitapp::machine_impl::config_file() const
{
	return config_file_;
}

sge::systems::instance const &
fruitapp::machine_impl::systems() const
{
	return systems_;
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

void
fruitapp::machine_impl::run_once()
{
	sge::time::point const before_frame = 
		sge::time::clock::now();
	systems_.window().dispatch();
	react(
		fruitlib::scenic::events::update(
			fruitlib::scenic::update_duration(
				static_cast<fruitlib::scenic::update_duration::rep>(
					second_timer_.reset()))));
	fcppt::chrono::milliseconds const diff = 
		fcppt::chrono::duration_cast<fcppt::chrono::milliseconds>(
			sge::time::clock::now() - before_frame);
	if (diff.count() < static_cast<fcppt::chrono::milliseconds::rep>(1000/desired_fps_))
		fcppt::time::sleep_any(
			fcppt::chrono::milliseconds(
				static_cast<fcppt::chrono::milliseconds::rep>(1000/desired_fps_ - diff.count())));
}

sge::time::callback const 
fruitapp::machine_impl::timer_callback() const
{
	return 
		std::tr1::bind(
			&sge::time::duration::count,
			std::tr1::bind(
				&sge::time::point::time_since_epoch,
				&transformed_time_));
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

sge::camera::object &
fruitapp::machine_impl::camera()
{
	return camera_;
}

sge::camera::object const &
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

sge::time::funit
fruitapp::machine_impl::time_factor() const
{
	return time_factor_;
}

void
fruitapp::machine_impl::time_factor(
	sge::time::funit const _time_factor)
{
	time_factor_ = 
		_time_factor;
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

fruitapp::machine_impl::~machine_impl()
{
}

void
fruitapp::machine_impl::react(
	fruitlib::scenic::events::update const &e)
{
	// So what does this do? Well, we effectively manage two "clocks"
	// here. One goes along with the real clock (with
	// sge::time::clock) and knows the "real" current time. The other
	// one (transformed_time) might be faster or slower than the real
	// clock. The real clock acts as a "duration difference" giver.
	sge::time::point const latest_time = 
		sge::time::clock::now();

	sge::time::duration const diff = 
		sge::time::duration(
			static_cast<sge::time::timer::interval_type>(
				time_factor_ * 
				static_cast<sge::time::funit>(
					(latest_time - current_time_).count())));

	transformed_time_ += 
		diff;

	current_time_ = 
		latest_time;

	node_base::forward_to_children(
		e);
}

void
fruitapp::machine_impl::toggle_camera()
{
	camera_.active(
		!camera_.active());
}

void
fruitapp::machine_impl::viewport_change()
{
	camera_.projection_object(
		fruitlib::json::parse_projection(
			fruitlib::json::find_and_convert_member<sge::parse::json::object>(
				config_file_,
				fruitlib::json::path(FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("camera")
					/ FCPPT_TEXT("projection")),
			sge::renderer::aspect(
				sge::renderer::viewport_size(
					systems_.renderer()))));

	node_base::forward_to_children(
		fruitlib::scenic::events::viewport_change());
}


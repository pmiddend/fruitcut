#include "machine_impl.hpp"
#include "name.hpp"
#include "../fruitlib/json/parse_projection.hpp"
#include "../fruitlib/json/find_and_convert_member.hpp"
#include "../fruitlib/json/merge_trees.hpp"
#include "../fruitlib/json/merge_command_line_parameters.hpp"
#include "../fruitlib/create_command_line_parameters.hpp"
#include "../fruitlib/log/scoped_sequence_from_json.hpp"
#include "../fruitlib/rng_creator.hpp"
#include "../fruitlib/time_format/string_to_duration.hpp"
#include "light_source_from_json.hpp"
#include "load_user_config.hpp"
#include "../media_path.hpp"
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
#include <sge/image/colors.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/log/global_context.hpp>
#include <sge/md3/create.hpp>
#include <sge/parse/json/json.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/depth_stencil_buffer.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/texture/texture.hpp>
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
#include <fcppt/chrono/chrono.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/time/sleep_any.hpp>
#include <fcppt/tr1/functional.hpp>

fruitcut::app::machine_impl::machine_impl(
	int const argc,
	char *argv[])
:
	fruitlib::scenic::nodes::intrusive_group(),
	rng_creator_(
		static_cast<fruitlib::rng_creator::value_type>(
			fcppt::chrono::high_resolution_clock::now().time_since_epoch().count())),
	user_config_file_(
		app::load_user_config()),
	config_file_(
		fruitlib::json::merge_command_line_parameters(
			fruitlib::json::merge_trees(
				sge::parse::json::parse_file_exn(
					fruitcut::media_path()/FCPPT_TEXT("config.json")),
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
					app::name(),
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
		sge::md3::create()),
	scene_node_(
		systems_,
		config_file_),
	overlay_node_(),
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
	current_time_(
		sge::time::clock::now()),
	transformed_time_(
		current_time_),
	time_factor_(
		static_cast<sge::time::funit>(
			1)),
	sound_controller_(
		rng_creator_,
		fruitcut::media_path()/FCPPT_TEXT("sounds"),
		systems_.audio_loader(),
		systems_.audio_player(),
		sge::audio::pool::gain_factor(
			fruitlib::json::find_and_convert_member<sge::audio::pool::gain_factor::value_type>(
				config_file(),
				fruitlib::json::path(
					FCPPT_TEXT("effects-volume"))))),
	sound_controller_node_(
		sound_controller_),
	effects_volume_change_connection_(
		config_variables_.effects_volume().change_callback(
			std::tr1::bind(
				static_cast<void(fruitlib::audio::sound_controller::*)(sge::audio::scalar)>(
					&fruitlib::audio::sound_controller::gain_factor),
				&sound_controller_,
				std::tr1::placeholders::_1))),
	music_controller_(
		rng_creator_,
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
	music_controller_node_(
		music_controller_),
	music_volume_change_connection_(
		config_variables_.music_volume().change_callback(
			std::tr1::bind(
				static_cast<void(fruitlib::audio::music_controller::*)(sge::audio::scalar)>(
					&fruitlib::audio::music_controller::volume),
				&music_controller_,
				std::tr1::placeholders::_1))),
	quick_log_(
		config_file_,
		font_cache_,
		systems_.viewport_manager(),
		systems_.renderer(),
		sound_controller_),
	camera_(
		sge::camera::parameters(
			// Leave projection object empty for now, we have to wait for a viewport change
			sge::camera::projection::object(),
			fruitlib::json::find_and_convert_member<sge::renderer::scalar>(
				config_file_,
				fruitlib::json::path(FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("camera")
					/ FCPPT_TEXT("movement-speed")),
			// mousespeed
			fruitlib::json::find_and_convert_member<sge::renderer::scalar>(
				config_file_,
				fruitlib::json::path(FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("camera")
					/ FCPPT_TEXT("mouse-speed")),
			// position
			sge::camera::identity_gizmo()
				.position(
					fruitlib::json::find_and_convert_member<sge::renderer::vector3>(
						config_file_,
						fruitlib::json::path(FCPPT_TEXT("ingame"))
							/ FCPPT_TEXT("camera")
							/ FCPPT_TEXT("initial-position"))),
			// Maus und Keyboard
			systems_.keyboard_collector(),
			systems_.mouse_collector(),
			sge::camera::activation_state::inactive)),
	camera_node_(
		camera_,
		timer_callback()),
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
		app::light_source_from_json(
			fruitlib::json::find_and_convert_member<sge::parse::json::object>(
				config_file_,
				fruitlib::json::path(
					FCPPT_TEXT("main-light-source"))))),
	shadow_map_(
		config_file_,
		systems_.renderer(),
		main_light_source_.model_view()),
	background_(
		systems_.renderer(),
		systems_.viewport_manager(),
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
				media_path()/FCPPT_TEXT("fonts")),
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
		fruitcut::media_path()/FCPPT_TEXT("point_sprites"),
		rng_creator_,
		systems_.renderer(),
		systems_.image_loader(),
		camera_),
	screen_shooter_(
		systems_.keyboard_collector(),
		systems_.renderer(),
		systems_.image_loader(),
		quick_log_)
{
	intrusive_group::insert_dont_care(
		music_controller_node_);
	intrusive_group::insert_dont_care(
		sound_controller_node_);
	intrusive_group::insert_dont_care(
		scene_node_);
	intrusive_group::insert_dont_care(
		overlay_node_);
	intrusive_group::insert_dont_care(
		shadow_map_);
	scene_node_.push_front(
		background_);
	scene_node_.insert_dont_care(
		camera_node_);
	scene_node_.insert_dont_care(
		point_sprites_);
	overlay_node_.insert_dont_care(
		quick_log_);
	systems_.audio_player().gain(
		fruitlib::json::find_and_convert_member<sge::audio::scalar>(
			config_file(),
			fruitlib::json::path(
				FCPPT_TEXT("audio-volume"))));
}

sge::parse::json::object const &
fruitcut::app::machine_impl::config_file() const
{
	return config_file_;
}

sge::systems::instance const &
fruitcut::app::machine_impl::systems() const
{
	return systems_;
}

sge::md3::loader &
fruitcut::app::machine_impl::md3_loader()
{
	return *md3_loader_;
}

fruitcut::app::config_variables &
fruitcut::app::machine_impl::config_variables()
{
	return config_variables_;
}

fruitcut::app::config_variables const &
fruitcut::app::machine_impl::config_variables() const
{
	return config_variables_;
}

fruitcut::app::postprocessing &
fruitcut::app::machine_impl::postprocessing()
{
	return scene_node_.postprocessing();
}

void
fruitcut::app::machine_impl::run_once()
{
	sge::time::point const before_frame = 
		sge::time::clock::now();
	systems_.window().dispatch();
	update();
	render();
	fcppt::chrono::milliseconds const diff = 
		fcppt::chrono::duration_cast<fcppt::chrono::milliseconds>(
			sge::time::clock::now() - before_frame);
	if (diff.count() < static_cast<fcppt::chrono::milliseconds::rep>(1000/desired_fps_))
		fcppt::time::sleep_any(
			fcppt::chrono::milliseconds(
				static_cast<fcppt::chrono::milliseconds::rep>(1000/desired_fps_ - diff.count())));
}

sge::time::callback const 
fruitcut::app::machine_impl::timer_callback() const
{
	return 
		std::tr1::bind(
			&sge::time::duration::count,
			std::tr1::bind(
				&sge::time::point::time_since_epoch,
				&transformed_time_));
}

fruitcut::fruitlib::audio::sound_controller &
fruitcut::app::machine_impl::sound_controller()
{
	return sound_controller_;
}

fruitcut::fruitlib::audio::sound_controller const &
fruitcut::app::machine_impl::sound_controller() const
{
	return sound_controller_;
}

fruitcut::fruitlib::audio::music_controller &
fruitcut::app::machine_impl::music_controller()
{
	return music_controller_;
}

fruitcut::fruitlib::audio::music_controller const &
fruitcut::app::machine_impl::music_controller() const
{
	return music_controller_;
}

fruitcut::app::background &
fruitcut::app::machine_impl::background()
{
	return background_;
}

fruitcut::app::background const &
fruitcut::app::machine_impl::background() const
{
	return background_;
}

fruitcut::app::directional_light_source const &
fruitcut::app::machine_impl::main_light_source()
{
	return main_light_source_;
}

fruitcut::app::shadow_map &
fruitcut::app::machine_impl::shadow_map()
{
	return shadow_map_;
}

fruitcut::app::shadow_map const &
fruitcut::app::machine_impl::shadow_map() const
{
	return shadow_map_;
}

sge::camera::object &
fruitcut::app::machine_impl::camera()
{
	return camera_;
}

sge::camera::object const &
fruitcut::app::machine_impl::camera() const
{
	return camera_;
}

fruitcut::fruitlib::font::cache &
fruitcut::app::machine_impl::font_cache()
{
	return font_cache_;
}

fruitcut::fruitlib::font::cache const &
fruitcut::app::machine_impl::font_cache() const
{
	return font_cache_;
}

sge::cegui::system &
fruitcut::app::machine_impl::gui_system()
{
	return gui_system_;
}

sge::cegui::system const &
fruitcut::app::machine_impl::gui_system() const
{
	return gui_system_;
}

sge::cegui::syringe &
fruitcut::app::machine_impl::gui_syringe()
{
	return gui_syringe_;
}

sge::cegui::syringe const &
fruitcut::app::machine_impl::gui_syringe() const
{
	return gui_syringe_;
}

fruitcut::fruitlib::rng_creator &
fruitcut::app::machine_impl::rng_creator()
{
	return rng_creator_;
}

fruitcut::app::score
fruitcut::app::machine_impl::last_game_score() const
{
	return last_game_score_;
}

void
fruitcut::app::machine_impl::last_game_score(
	score const &_last_game_score)
{
	last_game_score_ = _last_game_score;
}

fruitcut::app::scene &
fruitcut::app::machine_impl::scene_node()
{
	return scene_node_;
}

fruitcut::app::scene const &
fruitcut::app::machine_impl::scene_node() const
{
	return scene_node_;
}

fruitcut::app::overlay &
fruitcut::app::machine_impl::overlay_node()
{
	return overlay_node_;
}

fruitcut::app::overlay const &
fruitcut::app::machine_impl::overlay_node() const
{
	return overlay_node_;
}

fruitcut::app::point_sprite::system_node &
fruitcut::app::machine_impl::point_sprites() 
{
	return point_sprites_;
}

fruitcut::app::point_sprite::system_node const &
fruitcut::app::machine_impl::point_sprites() const 
{
	return point_sprites_;
}

sge::time::funit
fruitcut::app::machine_impl::time_factor() const
{
	return time_factor_;
}

void
fruitcut::app::machine_impl::time_factor(
	sge::time::funit const _time_factor)
{
	time_factor_ = 
		_time_factor;
}

fruitcut::app::fruit::prototype_sequence const &
fruitcut::app::machine_impl::fruit_prototypes() const
{
	return fruit_prototypes_;
}

fruitcut::app::fruit::prototype_sequence &
fruitcut::app::machine_impl::fruit_prototypes()
{
	return fruit_prototypes_;
}

fruitcut::app::quick_log &
fruitcut::app::machine_impl::quick_log()
{
	return quick_log_;
}

fruitcut::app::machine_impl::~machine_impl()
{
}

// FIXME: This could be a nice phoenix actor
void
fruitcut::app::machine_impl::toggle_camera()
{
	camera_.activation(
		camera_.activation() == sge::camera::activation_state::active
		?
			sge::camera::activation_state::inactive
		:
			sge::camera::activation_state::active);
}

void
fruitcut::app::machine_impl::viewport_change()
{
	scene_node_.postprocessing().viewport_changed();
	background_.viewport_changed();
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
}

void
fruitcut::app::machine_impl::update()
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

	intrusive_group::update();
}

void
fruitcut::app::machine_impl::render()
{
	// Do we even have a viewport?
	if (!sge::renderer::viewport_size(systems_.renderer()).content())
		return;

	sge::renderer::scoped_block scoped_block(
		systems_.renderer());

	intrusive_group::render();
}

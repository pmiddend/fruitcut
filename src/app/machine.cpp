#include "machine.hpp"
#include "name.hpp"
#include "config_wrapper.hpp"
#include "events/render.hpp"
#include "events/render_overlay.hpp"
#include "events/tick.hpp"
#include "../fruitlib/time_format/string_to_duration.hpp"
#include "../fruitlib/pp/dependency_set.hpp"
#include "../fruitlib/json/find_member.hpp"
#include "../fruitlib/log/scoped_sequence_from_json.hpp"
#include "../media_path.hpp"
#include <sge/audio/player.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/camera/identity_gizmo.hpp>
#include <sge/camera/parameters.hpp>
#include <sge/camera/projection/perspective.hpp>
#include <sge/cegui/cursor_visibility.hpp>
#include <sge/cegui/load_context.hpp>
#include <sge/console/sprite_object.hpp>
#include <sge/console/sprite_parameters.hpp>
#include <sge/extension_set.hpp>
#include <sge/font/size_type.hpp>
#include <sge/font/system.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image/colors.hpp>
#include <sge/input/keyboard/keyboard.hpp>
#include <sge/log/global_context.hpp>
#include <sge/log/global_context.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/depth_stencil_buffer.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/texture/texture.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/visual_depth.hpp>
#include <sge/renderer/vsync.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/systems/systems.hpp>
#include <sge/systems/window.hpp>
#include <sge/texture/texture.hpp>
#include <sge/time/clock.hpp>
#include <sge/viewport/fill_on_resize.hpp>
#include <sge/viewport/manager.hpp>
#include <sge/window/dim.hpp>
#include <sge/window/instance.hpp>
#include <sge/window/simple_parameters.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/chrono/chrono.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/dim/quad.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/move.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/time/sleep_any.hpp>
#include <fcppt/tr1/functional.hpp>
#include <iostream>

fruitcut::app::machine::machine(
	int argc,
	char *argv[])
:
	fruitlib::scenic::nodes::intrusive_group(),
	// We init that in run()
	running_(),
	config_file_(
		config_wrapper(
			argc,
			argv)),
	systems_(
		sge::systems::list()
			(sge::systems::window(
				sge::window::simple_parameters(
					name(),
					fruitlib::json::find_member<sge::window::dim>(
						config_file_,
						FCPPT_TEXT("graphics/window-size")))))
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
					fruitlib::json::find_member<bool>(
						config_file_,
						FCPPT_TEXT("mouse/confine-cursor"))
					?
						sge::systems::cursor_option_field(
							sge::systems::cursor_option::confine)
					:
						sge::systems::cursor_option_field())) 
			(sge::systems::audio_player_default())
			(sge::systems::parameterless::charconv)
			(sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>
					(FCPPT_TEXT("wav"))
					(FCPPT_TEXT("ogg"))))
			(sge::systems::parameterless::font) 	
			(sge::systems::parameterless::md3_loader) 	
			(sge::systems::image_loader(
					sge::image::capabilities_field::null(),
					fcppt::assign::make_container<sge::extension_set>
						(FCPPT_TEXT("png"))))),
	console_object_(
		SGE_FONT_TEXT_LIT('/')),
	scene_node_(
		systems_,
		console_object_,
		config_file_),
	overlay_node_(),
	activated_loggers_(
		fruitlib::log::scoped_sequence_from_json(
			sge::log::global_context(),
			fruitlib::json::find_member<sge::parse::json::array>(
				config_file_,
				FCPPT_TEXT("loggers/sge")))),
	font_cache_(
		systems_.font_system(),
		systems_.renderer(),
		systems_.image_loader(),
		fruitcut::media_path(),
		fruitlib::json::find_member<sge::parse::json::object>(
			config_file_,
			FCPPT_TEXT("fonts"))),
	input_manager_(
		systems_),
	console_state_(
		input_manager_),
	game_state_(
		input_manager_),
	previous_state_(
		0),
	console_font_(
		systems_.font_system().create_font(
			media_path()
				/ FCPPT_TEXT("fonts")
				/ 
					fruitlib::json::find_member<fcppt::string>(
						config_file(),
						FCPPT_TEXT("console/font")),
			fruitlib::json::find_member<sge::font::size_type>(
				config_file(),
				FCPPT_TEXT("console/font-size")))),
	console_gfx_(
		console_object_,
		systems_.renderer(),
		sge::image::colors::black(),
		*console_font_,
		console_state_,
		sge::console::sprite_object(
      sge::console::sprite_parameters()
      .texture(
				sge::texture::part_ptr(
					fcppt::make_shared_ptr<sge::texture::part_raw>(
						sge::renderer::texture::create_planar_from_view(
							systems_.renderer(),
							systems_.image_loader().load(
								media_path()
								/ FCPPT_TEXT("textures")
								/ 
									fruitlib::json::find_member<fcppt::string>(
										config_file(),
										FCPPT_TEXT("console/background-texture")))->view(),
							sge::renderer::texture::filter::linear,
							sge::renderer::texture::address_mode2(
								sge::renderer::texture::address_mode::clamp),
							sge::renderer::resource_flags::none))))
      .pos(
        sge::console::sprite_object::vector::null())
      .size(
				// We cannot specify a dimension here since we don't have a viewport yet
        sge::console::sprite_object::dim::null())
      .elements()),
		fruitlib::json::find_member<sge::console::output_line_limit>(
			config_file(),
			FCPPT_TEXT("console/line-limit"))),
	console_node_(
		console_gfx_),
	exit_connection_(
		systems_.keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				sge::systems::running_to_false(
					running_)))),
	current_time_(
		sge::time::clock::now()),
	transformed_time_(
		current_time_),
	time_factor_(
		static_cast<sge::time::funit>(
			1)),
	console_switch_connection_(
		systems_.keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f1,
				std::tr1::bind(
					&machine::console_switch,
					this)))),
	sound_controller_(
		fruitcut::media_path()/FCPPT_TEXT("sounds"),
		systems_.audio_loader(),
		systems_.audio_player()),
	sound_controller_node_(
		sound_controller_),
	music_controller_(
		systems_.audio_loader(),
		systems_.audio_player(),
		*fruitlib::time_format::string_to_duration<sge::time::duration>(
			fruitlib::json::find_member<fcppt::string>(
				config_file(),
				FCPPT_TEXT("music/crossfade-time"))),
		fruitcut::media_path()/FCPPT_TEXT("music"),
		fruitlib::json::find_member<sge::audio::scalar>(
			config_file(),
			FCPPT_TEXT("music/volume"))),
	music_controller_node_(
		music_controller_),
	camera_(
		sge::camera::parameters(
			// Leave projection object empty for now, we have to wait for a viewport change
			sge::camera::projection::object(),
			fruitlib::json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("ingame/camera/movement-speed")),
			// mousespeed
			fruitlib::json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("ingame/camera/mouse-speed")),
			// position
			sge::camera::identity_gizmo()
				.position(
					fruitlib::json::find_member<sge::renderer::vector3>(
						context<machine>().config_file(),
						FCPPT_TEXT("ingame/camera/initial-position"))),
			// Maus und Keyboard
			context<machine>().game_input_state(),
			context<machine>().game_input_state(),
			sge::camera::activation_state::inactive)),
	camera_node_(
		camera_,
		context<machine>().timer_callback()),
	toggle_camera_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f2, 
				std::tr1::bind(
					&machine::toggle_camera,
					this)))),
	camera_state_(
		context<machine>().input_manager()),
	viewport_change_connection_(
		systems_.viewport_manager().manage_callback(
			std::tr1::bind(
				&machine::viewport_change,
				this))),
	background_(
		systems_.renderer(),
		systems_.viewport_manager(),
		systems_.image_loader(),
		config_file_,
		camera_),
	desired_fps_(
		fruitlib::json::find_member<fcppt::chrono::milliseconds::rep>(
			config_file(),
			FCPPT_TEXT("graphics/desired-fps"))),
	gui_system_(
		sge::cegui::load_context(
			media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("fruitcut.scheme"))
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
		systems_.renderer(),
		systems_.image_loader(),
		camera_)
{
	intrusive_group::insert_dont_care(
		music_controller_node_);
	intrusive_group::insert_dont_care(
		sound_controller_node_);
	intrusive_group::insert_dont_care(
		scene_node_);
	intrusive_group::insert_dont_care(
		overlay_node_);
	scene_node_.push_front(
		background_);
	scene_node_.insert_dont_care(
		camera_node_);
	scene_node_.insert_dont_care(
		point_sprites_);
	overlay_node_.push_back(
		console_node_);
	input_manager_.current_state(
		game_state_);
	systems_.audio_player().gain(
		fruitlib::json::find_member<sge::audio::scalar>(
			config_file(),
			FCPPT_TEXT("audio-volume")));
}

sge::parse::json::object const &
fruitcut::app::machine::config_file() const
{
	return config_file_;
}

sge::systems::instance const &
fruitcut::app::machine::systems() const
{
	return systems_;
}

fruitcut::app::postprocessing &
fruitcut::app::machine::postprocessing()
{
	return scene_node_.postprocessing();
}

void
fruitcut::app::machine::run()
{
	running_ = true;
	while (running_)
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
}

sge::time::callback const 
fruitcut::app::machine::timer_callback() const
{
	return 
		/* Why doesn't this work?
		boost::phoenix::bind(
			&sge::time::duration::count,
			boost::phoenix::bind(
				&sge::time::point::time_since_epoch,
				&transformed_time_));
		*/
		std::tr1::bind(
			&sge::time::duration::count,
			std::tr1::bind(
				&sge::time::point::time_since_epoch,
				&transformed_time_));
}

fruitcut::fruitlib::audio::sound_controller &
fruitcut::app::machine::sound_controller()
{
	return sound_controller_;
}

fruitcut::fruitlib::audio::sound_controller const &
fruitcut::app::machine::sound_controller() const
{
	return sound_controller_;
}

fruitcut::fruitlib::audio::music_controller &
fruitcut::app::machine::music_controller()
{
	return music_controller_;
}

fruitcut::fruitlib::audio::music_controller const &
fruitcut::app::machine::music_controller() const
{
	return music_controller_;
}

fruitcut::fruitlib::input::state &
fruitcut::app::machine::game_input_state()
{
	return game_state_;
}

fruitcut::fruitlib::input::state_manager &
fruitcut::app::machine::input_manager()
{
	return input_manager_;
}

fruitcut::app::background &
fruitcut::app::machine::background()
{
	return background_;
}

fruitcut::app::background const &
fruitcut::app::machine::background() const
{
	return background_;
}

sge::camera::object &
fruitcut::app::machine::camera()
{
	return camera_;
}

sge::camera::object const &
fruitcut::app::machine::camera() const
{
	return camera_;
}

fruitcut::fruitlib::font::cache &
fruitcut::app::machine::font_cache()
{
	return font_cache_;
}

fruitcut::fruitlib::font::cache const &
fruitcut::app::machine::font_cache() const
{
	return font_cache_;
}

sge::cegui::system &
fruitcut::app::machine::gui_system()
{
	return gui_system_;
}

sge::cegui::system const &
fruitcut::app::machine::gui_system() const
{
	return gui_system_;
}

sge::cegui::syringe &
fruitcut::app::machine::gui_syringe()
{
	return gui_syringe_;
}

sge::cegui::syringe const &
fruitcut::app::machine::gui_syringe() const
{
	return gui_syringe_;
}

fruitcut::app::score
fruitcut::app::machine::last_game_score() const
{
	return last_game_score_;
}

void
fruitcut::app::machine::last_game_score(
	score const &_last_game_score)
{
	last_game_score_ = _last_game_score;
}

void
fruitcut::app::machine::quit()
{
	running_ = false;
}

fruitcut::app::scene &
fruitcut::app::machine::scene_node()
{
	return scene_node_;
}

fruitcut::app::scene const &
fruitcut::app::machine::scene_node() const
{
	return scene_node_;
}

fruitcut::app::overlay &
fruitcut::app::machine::overlay_node()
{
	return overlay_node_;
}

fruitcut::app::overlay const &
fruitcut::app::machine::overlay_node() const
{
	return overlay_node_;
}

fruitcut::app::point_sprite::system_node &
fruitcut::app::machine::point_sprites() 
{
	return point_sprites_;
}

fruitcut::app::point_sprite::system_node const &
fruitcut::app::machine::point_sprites() const 
{
	return point_sprites_;
}

sge::time::funit
fruitcut::app::machine::time_factor() const
{
	return time_factor_;
}

void
fruitcut::app::machine::time_factor(
	sge::time::funit const _time_factor)
{
	time_factor_ = 
		_time_factor;
}

fruitcut::app::machine::~machine()
{
}

void
fruitcut::app::machine::console_switch()
{
	console_gfx_.active(
		!console_gfx_.active());

	if (console_gfx_.active())
	{
		previous_state_ = 
			input_manager_.current_state();
		input_manager_.current_state(
			console_state_);
	}
	else
	{
		input_manager_.current_state(
			*previous_state_);
	}
}

// FIXME: This could be a nice phoenix actor
void
fruitcut::app::machine::toggle_camera()
{
	camera_.activation(
		camera_.activation() == sge::camera::activation_state::active
		?
			sge::camera::activation_state::inactive
		:
			sge::camera::activation_state::active);
}

void
fruitcut::app::machine::viewport_change()
{
	scene_node_.postprocessing().viewport_changed();
	console_gfx_.background_sprite().size(
		fcppt::math::dim::structure_cast<sge::console::sprite_object::dim>(
			systems_.renderer().onscreen_target().viewport().get().size()));
	background_.viewport_changed();
	camera_.projection_object(
		sge::camera::projection::perspective(
			sge::renderer::aspect(
				fcppt::math::dim::structure_cast<sge::renderer::screen_size>(
					context<machine>().systems().renderer().onscreen_target().viewport().get().size())),
			fcppt::math::deg_to_rad(
				fruitlib::json::find_member<sge::renderer::scalar>(
					context<machine>().config_file(),
					FCPPT_TEXT("ingame/camera/fov"))),
			fruitlib::json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("ingame/camera/near")),
			fruitlib::json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("ingame/camera/far"))));
}

void
fruitcut::app::machine::update()
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

	process_event(
		events::tick(
			diff));
}

void
fruitcut::app::machine::render()
{
	// Do we even have a viewport?
	if (!systems_.renderer().onscreen_target().viewport().get().size().content())
		return;

	sge::renderer::scoped_block scoped_block(
		systems_.renderer());

	intrusive_group::render();
}

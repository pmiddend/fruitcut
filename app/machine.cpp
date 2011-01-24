#include "machine.hpp"
#include "name.hpp"
#include "events/render.hpp"
#include "events/render_overlay.hpp"
#include "events/tick.hpp"
#include "../pp/dependency_set.hpp"
#include "../json/config_wrapper.hpp"
#include "../json/find_member.hpp"
#include "../media_path.hpp"
#include <sge/window/instance.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/systems/audio_player_default.hpp>
#include <sge/systems/cursor_grab.hpp>
#include <sge/systems/list.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/audio/player.hpp>
#include <sge/systems/window.hpp>
#include <sge/systems/renderer.hpp>
#include <sge/systems/viewport/manage_resize.hpp>
#include <sge/systems/input.hpp>
#include <sge/systems/input_helper_field.hpp>
#include <sge/systems/input_helper.hpp>
#include <sge/systems/image_loader.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/renderer/window_parameters.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/display_mode.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/bit_depth.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/depth_buffer.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/stencil_buffer.hpp>
#include <sge/renderer/window_mode.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/vsync.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/caps.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/format.hpp>
#include <sge/texture/rect_fragmented.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/console/sprite_object.hpp>
#include <sge/console/sprite_parameters.hpp>
#include <sge/image2d/file.hpp>
#include <sge/extension_set.hpp>
#include <sge/time/timer.hpp>
#include <sge/font/size_type.hpp>
#include <sge/time/unit.hpp>
#include <sge/time/second.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/font/system.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/time/clock.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <fcppt/math/dim/quad.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <string>

namespace
{
void
mytest(
	bool &r)
{
	r = false;
}
}

fruitcut::app::machine::machine(
	int argc,
	char *argv[])
:
	config_file_(
		json::config_wrapper(
			fcppt::assign::make_container<std::vector<fcppt::string> >(
				FCPPT_TEXT("user_config.json")),
		argc,
		argv)),
	systems_(
		sge::systems::list()
			(sge::systems::window(
				sge::renderer::window_parameters(
					name())))
			(sge::systems::renderer(
				sge::renderer::parameters(
					sge::renderer::display_mode(
						json::find_member<sge::renderer::screen_size>(
							config_file_,
							FCPPT_TEXT("graphics/screen-size")),
						sge::renderer::bit_depth::depth32,
						sge::renderer::refresh_rate_dont_care),
					sge::renderer::depth_buffer::d24,
					sge::renderer::stencil_buffer::off,
					sge::renderer::window_mode::windowed,
					sge::renderer::vsync::on,
					sge::renderer::no_multi_sampling),
				sge::systems::viewport::manage_resize()))
			(sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector) | sge::systems::input_helper::mouse_collector,
					sge::systems::cursor_option_field(
						sge::systems::cursor_option::confine))) 
			(sge::systems::audio_player_default())
			(sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>
					//(FCPPT_TEXT("ogg"))
					(FCPPT_TEXT("wav"))))
			(sge::systems::parameterless::font) 	
			(sge::systems::image_loader(
					sge::image::capabilities_field::null(),
					fcppt::assign::make_container<sge::extension_set>
						(FCPPT_TEXT("png"))))),
	texture_manager_(
		systems_.renderer(),
		boost::phoenix::construct<sge::texture::fragmented_unique_ptr>(
			boost::phoenix::new_<sge::texture::rect_fragmented>(
				systems_.renderer(),
				sge::image::color::format::rgba8,
				sge::renderer::filter::linear,
				fcppt::math::dim::quad<sge::renderer::dim2>(
					1024)))),
	input_manager_(
		systems_),
	console_state_(
		input_manager_),
	game_state_(
		input_manager_),
	console_object_(
		SGE_FONT_TEXT_LIT('/')),
	console_gfx_(
		console_object_,
		systems_.renderer(),
		sge::image::colors::black(),
		systems_.font_system()->create_font(
			media_path()
				/ FCPPT_TEXT("fonts")
				/ 
					json::find_member<fcppt::string>(
						config_file(),
						FCPPT_TEXT("console/font")),
			json::find_member<sge::font::size_type>(
				config_file(),
				FCPPT_TEXT("console/font-size"))),
		console_state_,
		sge::console::sprite_object(
      sge::console::sprite_parameters()
      .texture(
				create_single_texture(
					media_path()
						/ FCPPT_TEXT("textures")
						/ 
							json::find_member<fcppt::string>(
								config_file(),
								FCPPT_TEXT("console/background-texture"))))
      .pos(
        sge::console::sprite_object::point::null())
      .size(
        sge::console::sprite_object::dim(
					static_cast<sge::console::sprite_object::unit>(
						systems_.renderer()->screen_size().w()),
          static_cast<sge::console::sprite_object::unit>(
            systems_.renderer()->screen_size().h() / 2)))
      .elements()),
		json::find_member<sge::console::output_line_limit>(
			config_file(),
			FCPPT_TEXT("console/line-limit"))),
	postprocessing_(
		systems_.renderer(),
		console_object_,
		boost::bind(
			&machine::process_event,
			this,
			events::render()),
		json::find_member<sge::parse::json::object>(
			config_file(),
			FCPPT_TEXT("pp"))),
	particle_system_(
		systems_.renderer()),
	running_(
		true),
	exit_connection_(
		systems_.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				boost::bind(
					&mytest,
					boost::ref(running_))
				// This doesn't work, WHHYYYYYYYYY?
				//boost::phoenix::ref(running_) = false 
				))),
	current_time_(
		sge::time::clock::now()),
	transformed_time_(
		current_time_),
	time_transform_(
		boost::phoenix::arg_names::arg1),
	console_switch_connection_(
		systems_.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f1,
				boost::bind(
					&machine::console_switch,
					this)))),
	sound_controller_(
		json::find_member<sge::parse::json::object>(
			config_file(),
			FCPPT_TEXT("sounds")),
		systems_.audio_loader(),
		systems_.audio_player())
{
	input_manager_.current_state(
		game_state_);
	systems_.audio_player()->gain(
		json::find_member<sge::audio::scalar>(
			config_file(),
			FCPPT_TEXT("audio-volume")));
}

sge::parse::json::object const &
fruitcut::app::machine::config_file() const
{
	return config_file_;
}

sge::systems::instance const &
fruitcut::app::machine::systems()
{
	return systems_;
}

fruitcut::particle::system &
fruitcut::app::machine::particle_system()
{
	return particle_system_;
}

sge::texture::part_ptr const
fruitcut::app::machine::create_single_texture(
	fcppt::filesystem::path const &p)
{
	return 
		sge::texture::part_ptr(
			new sge::texture::part_raw(
				systems_.renderer()->create_texture(
					systems_.image_loader().load(
						p)->view(),
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none)));
}

sge::texture::part_ptr const
fruitcut::app::machine::create_texture(
	fcppt::filesystem::path const &p)
{
	return 
		sge::texture::add_image(
			texture_manager_,
			systems_.image_loader().load(
				p));
}

fruitcut::app::postprocessing &
fruitcut::app::machine::postprocessing()
{
	return postprocessing_;
}

void
fruitcut::app::machine::run()
{
	while (running_)
	{
		systems_.window()->dispatch();

		// So what does this do? Well, we effectively manage two "clocks"
		// here. One goes along with the real clock (with
		// sge::time::clock) and knows the "real" current time. The other
		// one (transformed_time) might be faster or slower than the real
		// clock. The real clock acts as a "duration difference" giver.
		sge::time::point const latest_time = 
			sge::time::clock::now();

		sge::time::duration const diff = 
			time_transform_(
				latest_time - current_time_);

		transformed_time_ += 
			diff;

		current_time_ = latest_time;

		process_event(
			events::tick(
				diff));

		sound_controller_.update();

		// This implicitly sends events::render through the
		// render-to-texture filter
		postprocessing_.update();

		sge::renderer::scoped_block scoped_block(
			systems_.renderer());

		postprocessing_.render_result();

		process_event(
			events::render_overlay());

		if (!console_gfx_.active())
			continue;

		console_gfx_.draw();
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
		boost::bind(
			&sge::time::duration::count,
			boost::bind(
				&sge::time::point::time_since_epoch,
				&transformed_time_));
}

void
fruitcut::app::machine::play_sound(
	fcppt::string const &name)
{
	sound_controller_.play(
		name);
}

fruitcut::input::state &
fruitcut::app::machine::game_input_state()
{
	return game_state_;
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
		input_manager_.current_state(
			console_state_);
	}
	else
		input_manager_.current_state(
			game_state_);
}

#include "machine.hpp"
#include "name.hpp"
#include "events/render.hpp"
#include "events/render_overlay.hpp"
#include "events/tick.hpp"
#include "events/viewport_change.hpp"
#include "../pp/dependency_set.hpp"
#include "../json/config_wrapper.hpp"
#include "../json/find_member.hpp"
#include "../media_path.hpp"
#include <sge/audio/player.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/console/sprite_object.hpp>
#include <sge/console/sprite_parameters.hpp>
#include <sge/exception.hpp>
#include <sge/extension_set.hpp>
#include <sge/font/size_type.hpp>
#include <sge/font/system.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image/colors.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/renderer/depth_buffer.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/optional_display_mode.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/stencil_buffer.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/texture/create_planar_from_view.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/vsync.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/systems/audio_player_default.hpp>
#include <sge/systems/image_loader.hpp>
#include <sge/systems/input_helper_field.hpp>
#include <sge/systems/input_helper.hpp>
#include <sge/systems/input.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/renderer.hpp>
#include <sge/systems/viewport/fill_on_resize.hpp>
#include <sge/systems/window.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/texture/rect_fragmented.hpp>
#include <sge/time/clock.hpp>
#include <sge/time/second.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/unit.hpp>
#include <sge/window/dim.hpp>
#include <sge/window/instance.hpp>
#include <sge/window/simple_parameters.hpp>
#include <awl/mainloop/asio/create_io_service.hpp>
#include <awl/mainloop/asio/io_service.hpp>
#include <awl/mainloop/io_service.hpp>
#include <awl/mainloop/dispatcher.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/math/dim/quad.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
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
	io_service_(
		awl::mainloop::asio::create_io_service()),
	systems_(
		sge::systems::list()
			(sge::systems::window(
				sge::window::simple_parameters(
					name(),
					json::find_member<sge::window::dim>(
						config_file_,
						FCPPT_TEXT("graphics/window-size")))).io_service(io_service_))
			(sge::systems::renderer(
				sge::renderer::parameters(
					sge::renderer::optional_display_mode(),
					sge::renderer::depth_buffer::d24,
					sge::renderer::stencil_buffer::off,
					sge::renderer::vsync::on,
					sge::renderer::no_multi_sampling),
				sge::systems::viewport::fill_on_resize()))
			(sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector) 
					| sge::systems::input_helper::mouse_collector
					| sge::systems::input_helper::cursor_demuxer,
					json::find_member<bool>(
						config_file_,
						FCPPT_TEXT("mouse/confine-cursor"))
					?
						sge::systems::cursor_option_field(
							sge::systems::cursor_option::confine)
					:
						sge::systems::cursor_option_field())) 
			(sge::systems::audio_player_default())
			(sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>
					(FCPPT_TEXT("wav"))))
			(sge::systems::parameterless::font) 	
			(sge::systems::parameterless::md3_loader) 	
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
				sge::renderer::texture::filter::linear,
				fcppt::math::dim::quad<sge::renderer::dim2>(
					1024)))),
	input_manager_(
		systems_),
	console_state_(
		input_manager_),
	game_state_(
		input_manager_),
	previous_state_(
		0),
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
								FCPPT_TEXT("console/background-texture")),
					sge::renderer::texture::address_mode::clamp))
      .pos(
        sge::console::sprite_object::point::null())
      .size(
				// We cannot specify a dimension here since we don't have a viewport yet
        sge::console::sprite_object::dim::null())
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
	exit_connection_(
		systems_.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				// boost::bind doesn't get this
				std::tr1::bind(
					&awl::mainloop::dispatcher::stop,
					systems_.window()->awl_dispatcher())))),
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
		systems_.audio_player()),
	frame_timer_(
		io_service_->get(),
		boost::posix_time::milliseconds(
			json::find_member<long>(
				config_file(),
				FCPPT_TEXT("frame-timer-ms")))),
	manage_viewport_connection_(
		systems_.manage_viewport_callback(
			boost::bind(
				&machine::manage_viewport,
				this,
				_1)))
{
	systems_.window()->show();
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
	fcppt::filesystem::path const &p,
	sge::renderer::texture::address_mode::type const address_mode)
{
	return 
		sge::texture::part_ptr(
			new sge::texture::part_raw(
				sge::renderer::texture::create_planar_from_view(
					systems_.renderer(),
					systems_.image_loader().load(
						p)->view(),
					sge::renderer::texture::filter::linear,
					sge::renderer::texture::address_mode2(
						address_mode),
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
	frame_timer_.expires_from_now(
		boost::posix_time::milliseconds(
			json::find_member<long>(
				config_file(),
				FCPPT_TEXT("frame-timer-ms"))));
	frame_timer_.async_wait(
		boost::bind(
			&machine::run_once,
			this,
			boost::asio::placeholders::error));
	io_service_->run();
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

fruitcut::input::state_manager &
fruitcut::app::machine::input_manager()
{
	return input_manager_;
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

void
fruitcut::app::machine::run_once(
	boost::system::error_code const &e)
{
	if (e)
		throw sge::exception(
			FCPPT_TEXT("Error in deadline_timer handler: ")+
			fcppt::from_std_string(
				e.message()));

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

	// Do we even have a viewport?
	if (systems_.renderer()->onscreen_target()->viewport().get().dimension().content())
	{
		// This implicitly sends events::render through the
		// render-to-texture filter
		postprocessing_.update();

		sge::renderer::scoped_block scoped_block(
			systems_.renderer());

		postprocessing_.render_result();

		process_event(
			events::render_overlay());

		if (console_gfx_.active())
			console_gfx_.draw();
	}

	if (!systems_.window()->awl_dispatcher()->is_stopped())
	{
		frame_timer_.expires_from_now(
			boost::posix_time::milliseconds(
				json::find_member<long>(
					config_file(),
					FCPPT_TEXT("frame-timer-ms"))));
		frame_timer_.async_wait(
			boost::bind(
				&machine::run_once,
				this,
				boost::asio::placeholders::error));
	}
}

void
fruitcut::app::machine::manage_viewport(
	sge::renderer::device_ptr)
{
	postprocessing_.viewport_changed();
	console_gfx_.background_sprite().size(
		fcppt::math::dim::structure_cast<sge::console::sprite_object::dim>(
			systems_.renderer()->onscreen_target()->viewport().get().dimension()));
	process_event(
		events::viewport_change());
}

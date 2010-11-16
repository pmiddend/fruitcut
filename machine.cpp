#include "machine.hpp"
#include "json/config_wrapper.hpp"
#include "json/find_member.hpp"
#include "events/tick.hpp"
#include "events/render.hpp"
#include "media_path.hpp"
#include "graphics/camera/parameters.hpp"
#include <sge/systems/list.hpp>
#include <sge/systems/window.hpp>
#include <sge/systems/renderer.hpp>
#include <sge/systems/input.hpp>
#include <sge/systems/parameterless.hpp>
#include <sge/systems/viewport/manage_resize.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/display_mode.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/bit_depth.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/depth_buffer.hpp>
#include <sge/renderer/stencil_buffer.hpp>
#include <sge/renderer/window_mode.hpp>
#include <sge/renderer/vsync.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/font/system.hpp>
#include <sge/font/size_type.hpp>
#include <sge/time/second.hpp>
#include <sge/window/instance.hpp>
#include <sge/input/keyboard/collector.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/console/sprite_object.hpp>
#include <sge/console/sprite_parameters.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/image/file.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/object.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/math/deg_to_rad.hpp>

fruitcut::machine::machine(
	int const argc,
	char *argv[])
:
	config_file_(
		json::config_wrapper(
			{},
			argc,
			argv)),
	systems_(
		sge::systems::list() 
		(sge::systems::window(
				sge::renderer::window_parameters(
				FCPPT_TEXT("fruitcut: test for cutting algorithm"))))
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
				sge::systems::input_helper::keyboard_collector) 
					| sge::systems::input_helper::mouse_collector))
		(sge::systems::parameterless::md3_loader)
		(sge::systems::image_loader(
			sge::image::capabilities_field::null(),
			{FCPPT_TEXT("png")}))
		(sge::systems::parameterless::font)),
	input_manager_(
		systems_),
	running_input_state_(
		input_manager_),
	console_input_state_(
		input_manager_),
	dead_(false),
	escape_connection_(
		systems_.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				[&dead_]() { dead_ = true; }))),
	camera_(
		graphics::camera::parameters(
			//graphics::camera::projection::orthogonal()
			graphics::camera::projection::perspective(
				sge::renderer::aspect<fruitcut::scalar>(
					systems_.renderer()->screen_size()),
				fcppt::math::deg_to_rad(
					fruitcut::json::find_member<fruitcut::scalar>(
						config_file_,
						FCPPT_TEXT("graphics/camera/fov"))),
				fruitcut::json::find_member<fruitcut::scalar>(
					config_file_,
					FCPPT_TEXT("graphics/camera/near")),
				fruitcut::json::find_member<fruitcut::scalar>(
					config_file_,
					FCPPT_TEXT("graphics/camera/far"))),
			fruitcut::json::find_member<fruitcut::scalar>(
				config_file_,
				FCPPT_TEXT("graphics/camera/movement-speed")),
			fruitcut::json::find_member<fruitcut::scalar>(
				config_file_,
				FCPPT_TEXT("graphics/camera/rotation-speed")),
			vec3::null(),
			running_input_state_)),
	frame_timer_(
		sge::time::second(1)),
	console_(
		FCPPT_TEXT('/')),
	console_gfx_(
		console_,
		systems_.renderer(),
		sge::image::colors::black(),
		systems_.font_system()->create_font(
			media_path()
			/ FCPPT_TEXT("fonts")
			/ FCPPT_TEXT("default.ttf"),
			static_cast<sge::font::size_type>(
				15)),
		systems_.keyboard_collector(),
		sge::console::sprite_object(
      sge::console::sprite_parameters()
      .texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						systems_.renderer()->create_texture(
							systems_.image_loader().load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("console_back.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
      .pos(
        sge::console::sprite_object::point::null())
      .size(
        sge::console::sprite_object::dim(
          systems_.renderer()->screen_size().w(),
          static_cast<sge::console::sprite_object::unit>(
            systems_.renderer()->screen_size().h() / 2)))
      .elements()),
		static_cast<sge::console::output_line_limit>(
			1000)),
	console_connection_(
		systems_.keyboard_collector()->key_callback(
			std::bind(
				&machine::console_callback,
				this,
				std::placeholders::_1)))
{
	input_manager_.current_state(
		running_input_state_);
}

bool
fruitcut::machine::dead() const
{
	return dead_;
}

void
fruitcut::machine::run_once()
{
	FCPPT_ASSERT(!dead_);
	systems_.window()->dispatch();
	process_event(
		events::tick(
			frame_timer_.reset()));
	render();
}

sge::parse::json::object const &
fruitcut::machine::config_file() const
{
	return config_file_;
}

sge::systems::instance const &
fruitcut::machine::systems() const
{
	return systems_;
}

fruitcut::graphics::camera::object const &
fruitcut::machine::camera() const
{
	return camera_;
}

fruitcut::graphics::camera::object &
fruitcut::machine::camera()
{
	return camera_;
}

void
fruitcut::machine::render()
{
	sge::renderer::scoped_block const block_(
		systems_.renderer());

	process_event(
		events::render());

	if (console_gfx_.active())
		console_gfx_.draw();
}

void
fruitcut::machine::console_callback(
	sge::input::keyboard::key_event const &e)
{
	if (!e.pressed())
		return;

	switch (e.key().code())
	{
		case sge::input::keyboard::key_code::f1:
			console_gfx_.active(
				!console_gfx_.active());
			input_manager_.current_state(
				console_gfx_.active() 
				? 
					console_input_state_
				:
					running_input_state_);
			break;
		default:
			break;
	}
}

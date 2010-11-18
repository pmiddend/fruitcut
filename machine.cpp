#include "machine.hpp"
#include "json/config_wrapper.hpp"
#include "json/find_member.hpp"
#include "events/tick.hpp"
#include "events/render.hpp"
#include "events/render_overlay.hpp"
#include "media_path.hpp"
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
#include <sge/renderer/device.hpp>
#include <sge/time/second.hpp>
#include <sge/window/instance.hpp>
#include <sge/input/keyboard/collector.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>

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
	dead_(false),
	escape_connection_(
		systems_.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				[&dead_]() { dead_ = true; }))),
	frame_timer_(
		sge::time::second(1))
{
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

void
fruitcut::machine::render()
{
	sge::renderer::scoped_block const block_(
		systems_.renderer());

	process_event(
		events::render());
	process_event(
		events::render_overlay());
}

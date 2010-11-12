#include "input/state_manager.hpp"
#include "input/state.hpp"
#include "input/optional_key_callback.hpp"
#include "input/optional_mouse_axis_callback.hpp"
#include "input/optional_mouse_button_callback.hpp"
#include <sge/log/global.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/renderer.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/input.hpp>
#include <sge/systems/parameterless.hpp>
#include <sge/systems/viewport/manage_resize.hpp>
#include <sge/input/keyboard/collector.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/device_ptr.hpp>
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
#include <sge/exception.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/text.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <exception>
#include <iostream>
#include <ostream>

int main()
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);

	sge::systems::instance sys(
		sge::systems::list() 
		(sge::window::parameters(
				FCPPT_TEXT("fruitcut: test for cutting algorithm")))
		(sge::systems::renderer(
			sge::renderer::parameters(
				sge::renderer::display_mode(
					sge::renderer::screen_size(
						640,480),
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
		(sge::systems::parameterless::md3_loader));
	
	bool running = 
		true;

	fcppt::signal::scoped_connection const cb(
		sys.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				[&running]() { running = false; })));

	fruitcut::input::state_manager input_manager(
		sys);

	fruitcut::input::state 
		console_state(
			input_manager),
		game_state(
			input_manager);

	fruitcut::input::state_connection console_connection = 
		console_state.create_connection(
			// Although no explicit constructor exists, we have to
			// explicitly write it here.
			fruitcut::input::optional_key_callback(
				[](sge::input::keyboard::key_event const &) 
				{
					std::cout << "console received a key\n";
				}),
			fruitcut::input::optional_mouse_axis_callback(),
			fruitcut::input::optional_mouse_button_callback());

	input_manager.current_state(
		game_state);

	fcppt::signal::scoped_connection const state_change_callback(
		sys.keyboard_collector()->key_callback(
			[&input_manager,&console_state,&game_state](
				sge::input::keyboard::key_event const &e) 
			{
				std::cout << "In state change callback\n";
				if (e.pressed() && e.key().code() == sge::input::keyboard::key_code::s)
				{
					std::cout << "Changing state\n";
					if (input_manager.current_state() == &console_state)
						input_manager.current_state(
							game_state);
					else
						input_manager.current_state(
							console_state);
				}
			}));

	while (running)
	{
		sge::mainloop::dispatch();

		sge::renderer::scoped_block const block_(
			sys.renderer());
	}
}
catch(sge::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch (fcppt::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch(std::exception const &e)
{
	std::cerr << e.what() << '\n';
	return EXIT_FAILURE;
}

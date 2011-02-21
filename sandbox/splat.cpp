#include "splatter.hpp"
#include <sge/image/capabilities.hpp>
#include <sge/image/colors.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/device.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/window.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/viewport/fill_on_resize.hpp>
#include <sge/window/instance.hpp>
#include <sge/window/dim.hpp>
#include <sge/window/simple_parameters.hpp>
#include <sge/log/global.hpp>
#include <sge/all_extensions.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/exception.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <cstdlib>
#include <exception>
#include <ostream>

int main()
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);

	sge::systems::instance sys(
		sge::systems::list()
		(sge::systems::window(
			sge::window::simple_parameters(
				FCPPT_TEXT("splat effect test"),
				sge::window::dim(
					1024,
					768))))
		(sge::systems::renderer(
			sge::renderer::parameters(
				sge::renderer::optional_display_mode(),
				sge::renderer::depth_buffer::off,
				sge::renderer::stencil_buffer::off,
				sge::renderer::vsync::on,
				sge::renderer::no_multi_sampling),
			sge::systems::viewport::fill_on_resize()))
		(sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector) 
					| sge::systems::input_helper::mouse_collector,
				sge::systems::cursor_option_field(
					sge::systems::cursor_option::confine) | sge::systems::cursor_option::hide)) 
		(sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				sge::all_extensions)));

	sys.window()->dispatch();

	sge::renderer::device_ptr const rend(
		sys.renderer());

	bool running = true;

	fcppt::signal::scoped_connection const cb(
		sys.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				boost::phoenix::ref(running) = false)));

	rend->state(
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = sge::image::colors::black()));

	fruitcut::sandbox::splatter splatter_(
		sys.renderer(),
		sys.image_loader(),
		*sys.mouse_collector());

	while(running)
	{
		sys.window()->dispatch();

		splatter_.update();

		sge::renderer::scoped_block const block_(
			rend);

		splatter_.render();
	}
}
catch(fcppt::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}

#include "font_drawer.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/viewport/manage_resize.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/font/bitmap/create.hpp>
#include <sge/font/text/drawer_3d.hpp>
#include <sge/font/text/part.hpp>
#include <sge/font/text/draw.hpp>
#include <sge/font/text/flags_none.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/config/media_path.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/window/instance.hpp>
#include <sge/extension_set.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <iostream>
#include <ostream>
#include <exception>
#include <cstdlib>

#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/dim.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>

namespace
{
std::pair<sge::font::pos,sge::font::dim> const
font_transformation(
	sge::font::pos const &total_pos,
	sge::font::dim const &total_size,
	sge::font::pos const &character_pos,
	sge::font::dim const &character_dim)
{
	static sge::time::timer frame_timer(sge::time::second(1));
	
	sge::font::pos const c = 
		total_pos + total_size/2;
	frame_timer.update();
	double const s = 
		static_cast<double>(
			frame_timer.elapsed_frames());
	return 
		std::make_pair(
			sge::font::pos(
				static_cast<sge::font::pos::value_type>(
					c.x() - c.x() * s + s * character_pos.x()),
				static_cast<sge::font::pos::value_type>(
					c.y() - c.y() * s + s * character_pos.y())),
			sge::font::dim(
				static_cast<sge::font::pos::value_type>(
					s * character_dim.w()),
				static_cast<sge::font::pos::value_type>(
					s * character_dim.h())));
}
}

int main()
try
{
	sge::systems::instance const sys(
		sge::systems::list()
		(
			sge::systems::window(
				sge::renderer::window_parameters(
					FCPPT_TEXT("sge animtest"))))
		(
			sge::systems::renderer(
				sge::renderer::parameters(
					sge::renderer::display_mode(
						sge::renderer::screen_size(
							1024,
							768),
						sge::renderer::bit_depth::depth32,
						sge::renderer::refresh_rate_dont_care),
					sge::renderer::depth_buffer::off,
					sge::renderer::stencil_buffer::off,
					sge::renderer::window_mode::windowed,
					sge::renderer::vsync::on,
					sge::renderer::no_multi_sampling),
				sge::systems::viewport::manage_resize()))
		(
			sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector)))
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>(
					FCPPT_TEXT("png")))));

	sge::font::metrics_ptr const font_metrics(
		sge::font::bitmap::create(
			sge::config::media_path()
			/ FCPPT_TEXT("fonts")
			/ FCPPT_TEXT("bitmap")
			/ FCPPT_TEXT("font.png"),
			sys.image_loader()));

	sge::font::text::drawer_ptr const font_drawer(
		fcppt::make_shared_ptr<fruitcut::sandbox::font_drawer>(
			sys.renderer(),
			sge::image::colors::white(),
			&font_transformation));

	sys.renderer()->state(
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = sge::image::colors::black()));

	bool running = true;

	fcppt::signal::scoped_connection const cb(
		sys.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				boost::phoenix::ref(running) = false)));

	while(running)
	{
		sys.window()->dispatch();

		sge::renderer::scoped_block const block_(
			sys.renderer());

		sge::font::text::draw(
			font_metrics,
			font_drawer,
			SGE_FONT_TEXT_LIT("test abcd"),
			sge::font::pos::null(),
			fcppt::math::dim::structure_cast<sge::font::dim>(
				sys.renderer()->screen_size()),
			sge::font::text::align_h::center,
			sge::font::text::align_v::center,
			sge::font::text::flags::none);
	}
}
catch(
	fcppt::exception const &_exception
)
{
	fcppt::io::cerr
		<< _exception.string()
		<< FCPPT_TEXT('\n');

	return EXIT_FAILURE;
}
catch(
	std::exception const &_exception
)
{
	std::cerr << _exception.what() << '\n';

	return EXIT_FAILURE;
}

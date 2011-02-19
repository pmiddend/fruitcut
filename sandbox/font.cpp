#include "../font/system.hpp"
#include "../font/particle/base_parameters.hpp"
#include "../font/color_format.hpp"
#include "../font/particle/animated.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/viewport/center_on_resize.hpp>
#include <sge/font/system.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/font/text/flags_none.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/config/media_path.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/window/instance.hpp>
#include <sge/extension_set.hpp>
#include <sge/time/second.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <iostream>
#include <ostream>
#include <exception>
#include <cstdlib>

int main()
try
{
	sge::systems::instance const sys(
		sge::systems::list()
		(sge::systems::window(
				sge::renderer::window_parameters(
					FCPPT_TEXT("fruitcut font test"))))
		(sge::systems::renderer(
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
				sge::systems::viewport::center_on_resize()))
		(sge::systems::parameterless::font)
		(sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector),
				sge::systems::cursor_option_field()))
		(sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>(
					FCPPT_TEXT("png")))));

	fruitcut::font::system font_system(
		sys.renderer(),
		sys.font_system(),
		sys.image_loader());

	sge::font::metrics_ptr 
		bitmap_metrics = 
			font_system.load_bitmap(
				sge::config::media_path()
					/ FCPPT_TEXT("fonts")
					/ FCPPT_TEXT("bitmap")
					/ FCPPT_TEXT("font.png")),
		ttf_metrics = 
			font_system.load_ttf(
				sge::config::media_path()
					/ FCPPT_TEXT("fonts")
					/ FCPPT_TEXT("default.ttf"),
				static_cast<sge::font::size_type>(
					32));

	FCPPT_ASSERT(
		font_system.load_bitmap(
			sge::config::media_path()
				/ FCPPT_TEXT("fonts")
				/ FCPPT_TEXT("bitmap")
				/ FCPPT_TEXT("font.png")) == bitmap_metrics);

	sys.renderer()->onscreen_target()->viewport(
		sge::renderer::viewport(
			sge::renderer::pixel_rect(
				sge::renderer::pixel_rect::vector::null(),
				fcppt::math::dim::structure_cast<sge::renderer::pixel_rect::dim>(
					sys.renderer()->screen_size()))));

	font_system.insert(
		fruitcut::font::particle::unique_base_ptr(
			new fruitcut::font::particle::animated(
				fruitcut::font::particle::base_parameters(
					bitmap_metrics,
					SGE_FONT_TEXT_LIT("Centered, should vanish soon"),
					fcppt::math::box::structure_cast<sge::font::rect>(
						sys.renderer()->onscreen_target()->viewport().get()),
					sge::font::text::align_h::center,
					sge::font::text::align_v::center,
					sge::font::text::flags::none),
				fcppt::assign::make_container<fruitcut::font::color_animation::value_sequence>
					(fruitcut::font::color_animation::value_type(
						sge::time::second(1),
						sge::image::color::any::convert<fruitcut::font::color_format>(
							sge::image::colors::white()))),
				fcppt::assign::make_container<fruitcut::font::scale_animation::value_sequence>
					(fruitcut::font::scale_animation::value_type(
						sge::time::second(1),
						static_cast<sge::renderer::scalar>(
							1))))));

	fruitcut::font::particle::animated ttf_font(
		fruitcut::font::particle::base_parameters(
			ttf_metrics,
			SGE_FONT_TEXT_LIT("Top left corner, should be permanent"),
			fcppt::math::box::structure_cast<sge::font::rect>(
				sys.renderer()->onscreen_target()->viewport().get()),
			sge::font::text::align_h::left,
			sge::font::text::align_v::top,
			sge::font::text::flags::none),
		fcppt::assign::make_container<fruitcut::font::color_animation::value_sequence>
			(fruitcut::font::color_animation::value_type(
				sge::time::second(1),
				sge::image::color::any::convert<fruitcut::font::color_format>(
					sge::image::colors::white()))),
		fcppt::assign::make_container<fruitcut::font::scale_animation::value_sequence>
			(fruitcut::font::scale_animation::value_type(
				sge::time::second(1),
				static_cast<sge::renderer::scalar>(
					1)))
			(fruitcut::font::scale_animation::value_type(
				sge::time::second(1),
				static_cast<sge::renderer::scalar>(
					0.5))));

	font_system.insert(
		ttf_font);

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

		font_system.update();
		font_system.render();
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

#include "../font/system.hpp"
#include "../font/particle/base_parameters.hpp"
#include "../font/color_format.hpp"
#include "../font/particle/animated.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/viewport/manage_resize.hpp>
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
#include <sge/image/color/rgba8.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/any/convert.hpp>
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
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <iostream>
#include <ostream>
#include <exception>
#include <cstdlib>

#include <fcppt/unique_ptr.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <boost/intrusive/link_mode.hpp>

#if 0
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/dim.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <cmath>

namespace
{
template<typename T>
T const
jag(
	T const &t)
{
	return 
		t < static_cast<T>(0.5) 
		? t
		: (static_cast<T>(1.0)-t);
}

template<typename T>
T const
bump(
	T const &t)
{
	return 
		std::abs(t) < static_cast<T>(1.0) 
		? std::exp(-static_cast<T>(1.0)/(static_cast<T>(1.0)-t*t))
		: 0;
}

sge::time::timer &global_timer()
{
	static sge::time::timer t(sge::time::second(1));
	return t;
}

sge::font::rect const
font_transformation(
	sge::font::rect const &total_rect,
	sge::font::rect const &character_rect)
{
	sge::font::pos const c = 
		total_rect.pos() + total_rect.dimension()/2;
	global_timer().update();
	double const s = 
		3.0 * bump(
			static_cast<double>(
				global_timer().elapsed_frames()) * 2.0 - 1.0);
	return 
		sge::font::rect(
			sge::font::pos(
				static_cast<sge::font::pos::value_type>(
					c.x() - c.x() * s + s * character_rect.pos().x()),
				static_cast<sge::font::pos::value_type>(
					c.y() - c.y() * s + s * character_rect.pos().y())),
			sge::font::dim(
				static_cast<sge::font::pos::value_type>(
					s * character_rect.dimension().w()),
				static_cast<sge::font::pos::value_type>(
					s * character_rect.dimension().h())));
}
}
#endif

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
				sge::systems::viewport::manage_resize()))
		(sge::systems::parameterless::font)
		(sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector),
				sge::systems::cursor_option_field()))
		(sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>(
					FCPPT_TEXT("png")))));

	/*
	sge::font::metrics_ptr const bitmap_metrics(
		sge::font::bitmap::create(
			sge::config::media_path()
			/ FCPPT_TEXT("fonts")
			/ FCPPT_TEXT("bitmap")
			/ FCPPT_TEXT("font.png"),
			sys.image_loader()));

	sge::font::metrics_ptr const ttf_metrics(
		sys.font_system()->create_font(
			sge::config::media_path()
			/ FCPPT_TEXT("fonts")
			/ FCPPT_TEXT("default.ttf"),
			static_cast<sge::font::size_type>(
				32)));
	*/

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

	sys.renderer()->onscreen_target()->viewport(
		sge::renderer::viewport(
			sge::renderer::pixel_rect(
				sge::renderer::pixel_rect::vector::null(),
				fcppt::math::dim::structure_cast<sge::renderer::pixel_rect::dim>(
					sys.renderer()->screen_size()))));

	font_system.insert(
		fruitcut::font::particle::unique_base_ptr(
			fcppt::make_unique_ptr<fruitcut::font::particle::animated>(
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
							sge::image::colors::white()))).container())));

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
					sge::image::colors::white()))));

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

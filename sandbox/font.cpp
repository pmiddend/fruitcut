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
#include <sge/image/color/init.hpp>
#include <sge/image/color/rgba8.hpp>
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

#include <fcppt/unique_ptr.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <boost/intrusive/link_mode.hpp>

namespace
{
class particle_base
{
FCPPT_NONCOPYABLE(
	particle_base);
public:
	explicit
	particle_base()
	{
	}

	virtual void
	update() = 0;

	virtual void
	render() = 0;

	virtual ~particle_base() {}
};

class intrusive_particle
:
	public
		particle_base,
	public 
		boost::intrusive::list_base_hook
		<
			boost::intrusive::link_mode<boost::intrusive::auto_unlink>
		>
{
public:	
};

class volatile_particle
:
	public particle_base
{
public:
	virtual bool
	dead() const = 0;
};

typedef
fcppt::unique_ptr<volatile_particle>
unique_volatile_particle_ptr;

class system
{
public:
	void
	insert(
		unique_volatile_particle_ptr o)
	{
		fcppt::container::ptr::push_back_unique_ptr(
			volatile_particles_,
			fcppt::move(
				o));
	}

	void
	insert(
		intrusive_particle &o)
	{
		intrusive_particles_.push_back(
			o);
	}

	void
	update()
	{
		for(
			volatile_particle_sequence::iterator i = 
				volatile_particles_.begin(); 
			i != volatile_particles_.end(); )
		{
			i->update();
			if (i->dead())
				i = 
					volatile_particles_.erase(
						i);
			else
				++i;
		}

		for(
			intrusive_particle_sequence::iterator i = 
				intrusive_particles_.begin(); 
			i != intrusive_particles_.end(); 
			++i)
			i->update();
	}

	void
	render()
	{
		for(
			volatile_particle_sequence::iterator i = 
				volatile_particles_.begin(); 
			i != volatile_particles_.end(); 
			++i)
			i->render();

		for(
			intrusive_particle_sequence::iterator i = 
				intrusive_particles_.begin(); 
			i != intrusive_particles_.end(); 
			++i)
			i->render();
	}
private:
	typedef
	boost::ptr_list<volatile_particle>
	volatile_particle_sequence;

	typedef
	boost::intrusive::list
	<
		intrusive_particle,
		boost::intrusive::constant_time_size<false>
	>
	intrusive_particle_sequence;

	volatile_particle_sequence volatile_particles_;
	intrusive_particle_sequence intrusive_particles_;
};

class pulsating
:
	public intrusive_particle
{
};
}

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

int main()
try
{
	sge::systems::instance const sys(
		sge::systems::list()
		(sge::systems::window(
				sge::renderer::window_parameters(
					FCPPT_TEXT("sge animtest"))))
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
		(sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector),
				sge::systems::cursor_option_field()))
		(sge::systems::image_loader(
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

		static_cast<fruitcut::sandbox::font_drawer &>(*font_drawer).color(
			sge::image::color::rgba8(
			(sge::image::color::init::red %= 1.0)
			(sge::image::color::init::green %= 1.0)
			(sge::image::color::init::blue %= 1.0)
			(sge::image::color::init::alpha %= (1.0 - global_timer().elapsed_frames()))));

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

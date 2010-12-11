#include "splat_collector.hpp"
#include "../sprite/system.hpp"
#include "../media_path.hpp"
#include <sge/config/media_path.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/system.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/input/mouse/device.hpp>
#include <sge/image/capabilities.hpp>
#include <sge/input/mouse/button_event.hpp>
#include <sge/input/mouse/axis_event.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/file.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/viewport/manage_resize.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/no_fragmented.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/time/second.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/time/default_callback.hpp>
#include <sge/log/global.hpp>
#include <sge/window/instance.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/renderer/device.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/all_extensions.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/random/uniform.hpp>
#include <fcppt/random/make_inclusive_range.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/bind.hpp>
#include <boost/next_prior.hpp>
#include <cstdlib>
#include <vector>
#include <exception>
#include <ostream>

namespace
{
class splatter
{
private:
	typedef
	std::vector<sge::renderer::texture_ptr>
	texture_vector;

	fruitcut::sandbox::splat_collector splat_collector_;
	fruitcut::sprite::system ss_;
	fruitcut::sprite::object cursor_;
	fruitcut::sprite::object canvas_;
	texture_vector textures_;
	fcppt::random::uniform<texture_vector::size_type> rng_;
	fcppt::signal::scoped_connection move_connection_,click_connection_;
public:
	explicit
	splatter(
		sge::renderer::device_ptr const _renderer,
		sge::image::multi_loader &_image_loader,
		sge::input::mouse::device &_mouse)
	:
		splat_collector_(
			_renderer),
		ss_(
			_renderer),
		cursor_(
			fruitcut::sprite::parameters()
				.texture_size()
				.visible(
					true)
				.texture(
					sge::texture::part_ptr(
						new sge::texture::part_raw(
							_renderer->create_texture(
								_image_loader.load(
									fruitcut::media_path() 
										/ FCPPT_TEXT("textures") 
										/ FCPPT_TEXT("cursor.png"))->view(),
								sge::renderer::filter::linear,
								sge::renderer::resource_flags::none))))
				.center(
					fruitcut::sprite::object::point(
						static_cast<fruitcut::sprite::object::unit>(
							_renderer->screen_size().w()/2),
						static_cast<fruitcut::sprite::object::unit>(
							_renderer->screen_size().h()/2)))
				.system(
					&ss_)
				.color(
					fruitcut::sprite::object::color_type(
						(sge::image::color::init::red %= 1.0)
						(sge::image::color::init::green %= 1.0)
						(sge::image::color::init::blue %= 1.0)
						(sge::image::color::init::alpha %= 1.0))).elements()),
		canvas_(
			fruitcut::sprite::parameters()
				.texture_size()
				.visible(
					true)
				.texture(
					sge::texture::part_ptr(
						new sge::texture::part_raw(
							splat_collector_.texture())))
				.pos(
					fruitcut::sprite::object::point::null())
				.system(
					&ss_)
				.color(
					fruitcut::sprite::object::color_type(
						(sge::image::color::init::red %= 1.0)
						(sge::image::color::init::green %= 1.0)
						(sge::image::color::init::blue %= 1.0)
						(sge::image::color::init::alpha %= 1.0))).elements()),
		textures_(
			load_textures(
				_renderer,
				_image_loader)),
		rng_(
			fcppt::random::make_inclusive_range(
				static_cast<texture_vector::size_type>(0),
				static_cast<texture_vector::size_type>(
					textures_.size()-1))),
		move_connection_(
			_mouse.axis_callback(
				boost::bind(
					&splatter::move_callback,
					this,
					_1))),
		click_connection_(
			_mouse.button_callback(
				boost::bind(
					&splatter::click_callback,
					this,
					_1)))
	{
	}

	void
	update()
	{
		splat_collector_.update();
	}

	void
	render()
	{
		ss_.render_all(
			sge::sprite::default_equal());
	}
private:
	void
	click_callback(
		sge::input::mouse::button_event const &e)
	{
		if (!e.pressed())
			return;

		splat_collector_.insert(
			fruitcut::sprite::parameters()
				.texture_size()
				.visible(
					true)
				.texture(
					sge::texture::part_ptr(
						new sge::texture::part_raw(
							*boost::next(
								textures_.begin(),
								rng_()))))
				.center(
					cursor_.pos() + cursor_.size()/2)
				.system(
					&ss_)
				.color(
					fruitcut::sprite::object::color_type(
						(sge::image::color::init::red %= 1.0)
						(sge::image::color::init::green %= 1.0)
						(sge::image::color::init::blue %= 1.0)
						(sge::image::color::init::alpha %= 1.0))));
	}

	void
	move_callback(
		sge::input::mouse::axis_event const &e)
	{
		cursor_.pos(
			cursor_.pos() + 
			fruitcut::sprite::object::point(
				e.axis() == sge::input::mouse::axis::x 
				? 
					static_cast<fruitcut::sprite::object::unit>(
						e.axis_value())
				:
					static_cast<fruitcut::sprite::object::unit>(
						0),
				e.axis() == sge::input::mouse::axis::x 
				? 
					static_cast<fruitcut::sprite::object::unit>(
						0)
				:
					static_cast<fruitcut::sprite::object::unit>(
						e.axis_value())));
	}

	texture_vector const
	load_textures(
		sge::renderer::device_ptr const renderer,
		sge::image::multi_loader &image_loader)
	{
		texture_vector t;
		for(
			fcppt::filesystem::directory_iterator it(
				fruitcut::media_path()
					/FCPPT_TEXT("textures")
					/FCPPT_TEXT("splatter")); 
			it != fcppt::filesystem::directory_iterator(); 
			++it)
			t.push_back(
				renderer->create_texture(
					image_loader.load(
						*it)->view(),
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none));
			
		return t;
	}
};
}

int main()
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug
	);

	sge::systems::instance sys(
		sge::systems::list()
		(
			sge::systems::window(
				sge::renderer::window_parameters(
					FCPPT_TEXT("splat test"))))
		(
			sge::systems::renderer(
				sge::renderer::parameters(
					sge::renderer::display_mode(
						sge::renderer::screen_size(
							1024,
							768
						),
						sge::renderer::bit_depth::depth32,
						sge::renderer::refresh_rate_dont_care
					),
					sge::renderer::depth_buffer::off,
					sge::renderer::stencil_buffer::off,
					sge::renderer::window_mode::windowed,
					sge::renderer::vsync::on,
					sge::renderer::no_multi_sampling
				),
				sge::systems::viewport::manage_resize()))
		(
			sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector) | sge::systems::input_helper::mouse_collector)) 
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				sge::all_extensions)));

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
			(sge::renderer::state::color::clear_color = sge::image::colors::black())
	);

	splatter splatter_(
		sys.renderer(),
		sys.image_loader(),
		*sys.mouse_collector());

	while(running)
	{
		sys.window()->dispatch();

		sge::renderer::scoped_block const block_(
			rend);

		splatter_.update();
		splatter_.render();
	}
}
catch(fcppt::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}

#include "../media_path.hpp"
#include "../grid/from_image.hpp"
#include "../grid/to_ppm.hpp"
#include "../grid/map.hpp"
#include "../grid/sobel_ignore_borders.hpp"
#include "../particle/point_sprite/system.hpp"
#include "../particle/point_sprite/object.hpp"
#include "../particle/point_sprite/parameters.hpp"
#include "../particle/sprite/system.hpp"
#include "../particle/sprite/object.hpp"
#include "../particle/sprite/parameters.hpp"
#include "../grid/collect_points.hpp"
#include "../particle/system.hpp"
#include "../particle/objects/simple.hpp"
#include "../animation.hpp"
#include <sge/sprite/intrusive/system_impl.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/image_loader.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image2d/file_ptr.hpp>
#include <sge/image/color/channel8.hpp>
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
#include <sge/all_extensions.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/time/second.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/window/instance.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second_f.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image/capabilities.hpp>
#include <sge/image/colors.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/viewport/manage_resize.hpp>
#include <sge/extension_set.hpp>
#include <sge/time/default_callback.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/clamp.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/comparison.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/object/static_cast.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <limits>
#include <iostream>
#include <algorithm>

namespace
{
typedef
fruitcut::animation<fruitcut::particle::sprite::object::color_type>
sprite_animation;

typedef
fruitcut::animation<fruitcut::particle::point_sprite::object::color_type>
point_sprite_animation;

typedef
float
scalar;

typedef
fcppt::container::grid::object
<
	sge::image::color::channel8,
	2
>
gray_grid;

typedef
fcppt::container::grid::object
<
	scalar,
	2
>
scalar_grid;

scalar_grid const
gray_grid_to_scalar_grid(
	gray_grid const &input)
{
	return 
		fruitcut::grid::map<scalar_grid::value_type>(
			input,
			boost::phoenix::static_cast_<scalar_grid::value_type>(
			boost::phoenix::arg_names::arg1 - std::numeric_limits<gray_grid::value_type>::min())/
			static_cast<scalar_grid::value_type>(
				std::numeric_limits<gray_grid::value_type>::max() - std::numeric_limits<gray_grid::value_type>::min()));
}

class explosion_particle
:
	public fruitcut::particle::objects::simple<fruitcut::particle::point_sprite::choices>
{
public:
	// gcc-4.4 doesn't like "simple(" in the constructor
	typedef
	fruitcut::particle::objects::simple<fruitcut::particle::point_sprite::choices>
	base;

	explicit
	explosion_particle(
		fruitcut::particle::point_sprite::parameters const &_params,
		point_sprite_animation::value_sequence const &_animation,
		sge::renderer::vector2 const &_velocity,
		sge::renderer::vector2 const &_acceleration,
		sge::time::duration const &_timer)
	:
		base(
			_params,
			_animation,
			sge::time::default_callback(),
			sge::renderer::vector2::null(),
			sge::renderer::vector2::null()),
		velocity_(
			_velocity),
		acceleration_(
			_acceleration),
		timer_(
			_timer)
	{
	}

	void
	update()
	{
		simple::update();
		
		if (timer_.expired() && timer_.active())
		{
			timer_.deactivate();
			velocity(
				velocity_);
			acceleration(
				acceleration_);
		}
	}
private:
	sge::renderer::vector2 const velocity_;
	sge::renderer::vector2 const acceleration_;
	sge::time::timer timer_;
};

class particles
{
public:
	explicit
	particles(
		sge::systems::instance const &);

	void
	from_image(
		sge::image2d::file_ptr);

	void
	update();

	void
	render();
private:
	fruitcut::particle::system system_;
	sge::texture::part_ptr particle_texture_;
	fruitcut::particle::point_sprite::object::point logo_pos_;
};

particles::particles(
	sge::systems::instance const &sys)
:
	system_(
		sys.renderer()),
	particle_texture_(
		new sge::texture::part_raw(
			sys.renderer()->create_texture(
				sys.image_loader().load(
					fruitcut::media_path() 
						/ FCPPT_TEXT("textures") 
						/ FCPPT_TEXT("smooth_particle.png"))->view(),
				sge::renderer::filter::linear,
				sge::renderer::resource_flags::none))),
	logo_pos_()
{
	sge::image2d::file_ptr const logo_image = 
		sys.image_loader().load(
			fruitcut::media_path() 
				/ FCPPT_TEXT("textures") 
				/ FCPPT_TEXT("logo.png"));

	logo_pos_ = 
		fruitcut::particle::point_sprite::object::point(
			static_cast<fruitcut::particle::point_sprite::object::unit>(
				sys.renderer()->screen_size().w()/2),
			static_cast<fruitcut::particle::point_sprite::object::unit>(
				sys.renderer()->screen_size().h()/2)) - 
		fcppt::math::dim::structure_cast<fruitcut::particle::point_sprite::object::point>(
			logo_image->dim())/static_cast<fruitcut::particle::point_sprite::object::point::value_type>(2);

	system_.insert(
		fruitcut::particle::objects::unique_base_ptr(
			new fruitcut::particle::objects::simple<fruitcut::particle::sprite::choices>(
				fruitcut::particle::sprite::parameters()
					.texture(
						sge::texture::part_ptr(
							new sge::texture::part_raw(
								sys.renderer()->create_texture(
									logo_image->view(),
									sge::renderer::filter::linear,
									sge::renderer::resource_flags::none))))
					.repetition(
						static_cast<fruitcut::particle::sprite::object::repetition_type>(
							1))
					.visible(
						true)
					.texture_size()
					.rotation(
						static_cast<fruitcut::particle::sprite::object::rotation_type>(
							0))
					.center(
						fruitcut::particle::sprite::object::point(
							static_cast<fruitcut::particle::sprite::object::unit>(
								sys.renderer()->screen_size().w()/2),
							static_cast<fruitcut::particle::sprite::object::unit>(
								sys.renderer()->screen_size().h()/2)))
					.system(
						&system_.sprite_system())
					.color(
						fruitcut::particle::sprite::object::color_type(
							(sge::image::color::init::red %= 1.0)
							(sge::image::color::init::green %= 1.0)
							(sge::image::color::init::blue %= 1.0)
							(sge::image::color::init::alpha %= 1.0))),
				fcppt::assign::make_container<sprite_animation::value_sequence>
					(sprite_animation::value_type(
						sge::time::second_f(
							static_cast<sge::time::funit>(
								3)),
						fruitcut::particle::sprite::object::color_type(
							(sge::image::color::init::red %= 1.0)
							(sge::image::color::init::green %= 1.0)
							(sge::image::color::init::blue %= 1.0)
							(sge::image::color::init::alpha %= 1.0))))
					(sprite_animation::value_type(
						sge::time::second_f(
							static_cast<sge::time::funit>(
								0)),
						fruitcut::particle::sprite::object::color_type(
							(sge::image::color::init::red %= 1.0)
							(sge::image::color::init::green %= 1.0)
							(sge::image::color::init::blue %= 1.0)
							(sge::image::color::init::alpha %= 0.0)))),
				sge::time::default_callback(),
				sge::renderer::vector2::null(),
				sge::renderer::vector2::null())));
}

void
particles::render()
{
	system_.render();
}

void
particles::from_image(
	sge::image2d::file_ptr const input_image)
{
	scalar_grid const g = 
		fruitcut::grid::map<scalar>(
			fruitcut::grid::sobel_ignore_borders(
				gray_grid_to_scalar_grid(
					fruitcut::grid::from_image<gray_grid::value_type>(
						input_image,
						sge::image::color::format::gray8))),
			boost::bind(
				&fcppt::math::clamp<scalar>,
				_1,
				static_cast<scalar>(0),
				static_cast<scalar>(1)));

	typedef
	std::vector<scalar_grid::dim>
	point_sequence;

	point_sequence points = 
		fruitcut::grid::collect_points<point_sequence>(
			g,
			boost::phoenix::arg_names::arg1 > static_cast<scalar>(0.5));

	std::random_shuffle(
		points.begin(),
		points.end());

	std::cout << "point size: " << points.size() << "\n";
	points.resize(
		std::min(
			static_cast<point_sequence::size_type>(
				800),
			points.size()));

	BOOST_FOREACH(
		point_sequence::const_reference p,
		points)
	{
		fruitcut::particle::point_sprite::object::point const center = 
			logo_pos_ + 
				fruitcut::particle::point_sprite::object::point(
					static_cast<fruitcut::particle::point_sprite::object::unit>(
						p.w()),
					static_cast<fruitcut::particle::point_sprite::object::unit>(
						p.h()));

		system_.insert(
			fruitcut::particle::objects::unique_base_ptr(
				new explosion_particle(
					fruitcut::particle::point_sprite::parameters()
						.texture(
							particle_texture_)
						.center(
							center)
						.point_size(
							static_cast<fruitcut::particle::point_sprite::object::unit>(
								particle_texture_->dim().w()))
						.system(
							&system_.point_sprite_system())
						.color(
							fruitcut::particle::point_sprite::object::color_type(
								(sge::image::color::init::red %= 1.0)
								(sge::image::color::init::green %= 1.0)
								(sge::image::color::init::blue %= 1.0)
								(sge::image::color::init::alpha %= 0.0))),
					fcppt::assign::make_container<point_sprite_animation::value_sequence>
						(point_sprite_animation::value_type(
							sge::time::second_f(
								static_cast<sge::time::funit>(
									2)),
							fruitcut::particle::point_sprite::object::color_type(
								(sge::image::color::init::red %= 1.0)
								(sge::image::color::init::green %= 1.0)
								(sge::image::color::init::blue %= 1.0)
								(sge::image::color::init::alpha %= 0.0))))
						(point_sprite_animation::value_type(
							sge::time::second_f(
								static_cast<sge::time::funit>(
									0.5)),
							fruitcut::particle::point_sprite::object::color_type(
								(sge::image::color::init::red %= 1.0)
								(sge::image::color::init::green %= 1.0)
								(sge::image::color::init::blue %= 1.0)
								(sge::image::color::init::alpha %= 1.0))))
						(point_sprite_animation::value_type(
							sge::time::second_f(
								static_cast<sge::time::funit>(
									1)),
							fruitcut::particle::point_sprite::object::color_type(
								(sge::image::color::init::red %= 1.0)
								(sge::image::color::init::green %= 1.0)
								(sge::image::color::init::blue %= 1.0)
								(sge::image::color::init::alpha %= 0.0)))),
					fcppt::math::vector::structure_cast<sge::renderer::vector2>(
						center) - 
					sge::renderer::vector2(
						static_cast<sge::renderer::scalar>(
							system_.sprite_system().renderer()->screen_size().w()/2),
						static_cast<sge::renderer::scalar>(
							system_.sprite_system().renderer()->screen_size().h()/2)),
					sge::renderer::vector2(
						0,
						0),
					sge::time::second_f(
						static_cast<sge::time::funit>(
							2.0)))));
	}
}

void
particles::update()
{
	system_.update();
}
}

int main()
try
{
	sge::systems::instance sys(
		sge::systems::list()
		(
			sge::systems::window(
				sge::renderer::window_parameters(
					FCPPT_TEXT("trail test"))))
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
	
	particles ps(
		sys);

	ps.from_image(
		sys.image_loader().load(
			fruitcut::media_path()
				/ FCPPT_TEXT("textures")
				/ FCPPT_TEXT("logo.png")));

	bool running = true;

	fcppt::signal::scoped_connection const cb(
		sys.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				boost::phoenix::ref(running) = false)));

	sys.renderer()->state(
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = sge::image::colors::black()));

	while(running)
	{
		sys.window()->dispatch();

		sge::renderer::scoped_block const block_(
			sys.renderer());

		ps.update();
		ps.render();
	}
}
catch(
	fcppt::exception const &e)
{
	fcppt::io::cout << "Exception: " << e.string() << "\n";
}


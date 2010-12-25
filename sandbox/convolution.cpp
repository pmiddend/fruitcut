#include "../media_path.hpp"
#include "../grid/from_image.hpp"
#include "../grid/to_ppm.hpp"
#include "../grid/map.hpp"
#include "../grid/sobel_ignore_borders.hpp"
#include "../sprite/system.hpp"
#include "../sprite/object.hpp"
#include "../sprite/parameters.hpp"
#include "../grid/collect_points.hpp"
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

class particle
{
public:
	explicit
	particle(
		fruitcut::sprite::parameters const &,
		sge::time::duration const &life_time,
		sge::renderer::vector2 const &velocity,
		sge::renderer::vector2 const &acceleration);

	void
	update();

	bool
	dead() const;

	fruitcut::sprite::object const &
	sprite() const;

	void
	acceleration(
		sge::renderer::vector2 const &);

	~particle();
private:
	fruitcut::sprite::object sprite_;
	sge::time::timer life_timer_;
	sge::time::timer seconds_timer_;
	sge::renderer::vector2 position_;
	sge::renderer::vector2 velocity_;
	sge::renderer::vector2 acceleration_;
};

particle::particle(
	fruitcut::sprite::parameters const &params,
	sge::time::duration const &_life_time,
	sge::renderer::vector2 const &_velocity,
	sge::renderer::vector2 const &_acceleration)
:
	sprite_(
		params.elements()),
	life_timer_(
		_life_time),
	seconds_timer_(
		sge::time::second_f(
			static_cast<sge::time::funit>(
				1))),
	position_(
		fcppt::math::vector::structure_cast<sge::renderer::vector2>(
			sprite_.pos())),
	velocity_(
		_velocity),
	acceleration_(
		_acceleration)
{
}

void
particle::update()
{
	sge::renderer::scalar const delta = 
		static_cast<sge::renderer::scalar>(
			seconds_timer_.reset());
	position_ += 
		delta * velocity_;
	velocity_ += 
		delta * acceleration_;
	sprite_.pos(
		fcppt::math::vector::structure_cast<fruitcut::sprite::object::point>(
			position_));
}

bool 
particle::dead() const
{
	return life_timer_.expired();
}

fruitcut::sprite::object const &
particle::sprite() const
{
	return sprite_;
}

void
particle::acceleration(
	sge::renderer::vector2 const &_acceleration)
{
	acceleration_ = _acceleration;
}

particle::~particle()
{
}

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
	typedef
	boost::ptr_list<particle>
	particle_sequence;

	fruitcut::sprite::system ss_;
	fruitcut::sprite::object logo_;

	sge::time::timer logo_timer_;

	sge::texture::part_ptr particle_texture_;
	particle_sequence particles_;
};

particles::particles(
	sge::systems::instance const &sys)
:
	ss_(
		sys.renderer()),
	logo_(
		fruitcut::sprite::parameters()
			.texture_size()
			.visible(
				true)
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						sys.renderer()->create_texture(
							sys.image_loader().load(
								fruitcut::media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("logo.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
			.center(
				fruitcut::sprite::object::point(
					static_cast<fruitcut::sprite::object::unit>(
						sys.renderer()->screen_size().w()/2),
					static_cast<fruitcut::sprite::object::unit>(
						sys.renderer()->screen_size().h()/2)))
			.system(
				&ss_)
			.color(
				fruitcut::sprite::object::color_type(
					(sge::image::color::init::red %= 1.0)
					(sge::image::color::init::green %= 1.0)
					(sge::image::color::init::blue %= 1.0)
					(sge::image::color::init::alpha %= 1.0))).elements()),
	logo_timer_(
		sge::time::second_f(
			static_cast<sge::time::funit>(
				1))),
	particle_texture_(
		new sge::texture::part_raw(
			sys.renderer()->create_texture(
				sys.image_loader().load(
					fruitcut::media_path() 
						/ FCPPT_TEXT("textures") 
						/ FCPPT_TEXT("smooth_particle.png"))->view(),
				sge::renderer::filter::linear,
				sge::renderer::resource_flags::none)))
{
}

void
particles::render()
{
	ss_.render_all(
		sge::sprite::default_equal());
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

	points.resize(200);

	BOOST_FOREACH(
		point_sequence::const_reference p,
		points)
		particles_.push_back(
			new particle(
				fruitcut::sprite::parameters()
					.texture_size()
					.visible(
						true)
					.texture(
						particle_texture_)
					.center(
						logo_.pos() + 
						fruitcut::sprite::object::point(
							static_cast<fruitcut::sprite::object::unit>(
								p.w()),
							static_cast<fruitcut::sprite::object::unit>(
								p.h())))
					.system(
						&ss_)
					.color(
						fruitcut::sprite::object::color_type(
							(sge::image::color::init::red %= 1.0)
							(sge::image::color::init::green %= 1.0)
							(sge::image::color::init::blue %= 1.0)
							(sge::image::color::init::alpha %= 1.0))),
				sge::time::second_f(
					static_cast<sge::time::funit>(
						5)),
				sge::renderer::vector2::null(),
				sge::renderer::vector2(
					0,
					/*20*/0)));
}

void
particles::update()
{
	logo_.color(
		(sge::image::color::init::red %= 1.0)
		(sge::image::color::init::green %= 1.0)
		(sge::image::color::init::blue %= 1.0)
		(sge::image::color::init::alpha %= 
			logo_timer_.expired() ? 0.0 : (static_cast<double>(1) - static_cast<double>(logo_timer_.elapsed_frames()))));

	if (logo_timer_.expired())
	{
		static bool inited = false;
		if (!inited)
		{
			for (particle_sequence::iterator i = particles_.begin(); i != particles_.end(); ++i)
				i->acceleration(sge::renderer::vector2(0,20));
			inited = true;
		}
	}

	for (particle_sequence::iterator i = particles_.begin(); i != particles_.end();)
	{
		i->update();
		if (i->dead())
			i = 
				particles_.erase(
					i);
		else
			++i;
	}
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


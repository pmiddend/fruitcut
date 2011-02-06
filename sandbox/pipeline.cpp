#include "../media_path.hpp"
#include "../pp/system.hpp"
#include "../pp/filter/blur.hpp"
#include "../pp/filter/highlight.hpp"
#include "../pp/filter/ssaa.hpp"
#include "../pp/filter/render_to_texture.hpp"
#include "../pp/filter/add.hpp"
#include "../pp/texture/manager.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/window/instance.hpp>
#include <sge/config/media_path.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <sge/font/text/to_fcppt_string.hpp>
#include <sge/font/text/from_fcppt_string.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/system.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/console/gfx.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/console/object.hpp>
#include <sge/log/global.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/mouse/axis_event.hpp>
#include <sge/input/mouse/device.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image/colors.hpp>
#include <sge/font/system.hpp>
#include <sge/font/size_type.hpp>
#include <sge/console/sprite_object.hpp>
#include <sge/console/sprite_parameters.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/object.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/filter/point.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image/color/object_impl.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/system.hpp>
#include <sge/sprite/external_system_impl.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/choices.hpp>
#include <sge/sprite/type_choices.hpp>
#include <sge/sprite/with_texture.hpp>
#include <sge/sprite/with_dim.hpp>
#include <sge/sprite/with_color.hpp>
#include <sge/sprite/with_depth.hpp>
#include <sge/sprite/default_sort.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/systems/viewport/manage_resize.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/no_fragmented.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/exception.hpp>
#include <sge/renderer/device.hpp>
#include <sge/extension_set.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/nonassignable.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <exception>
#include <ostream>
#include <vector>
#include <iostream>
#include <cstdlib>

namespace
{

typedef sge::image::color::rgba8_format sprite_color;

typedef sge::sprite::choices<
	sge::sprite::type_choices<
		int,
		float,
		sprite_color
	>,
	boost::mpl::vector4<
		sge::sprite::with_color,
		sge::sprite::with_texture,
		sge::sprite::with_dim,
		sge::sprite::with_depth
	> 
> sprite_choices;

typedef sge::sprite::object<
	sprite_choices
> sprite_object;

class sprite_functor
{
FCPPT_NONASSIGNABLE(
	sprite_functor);
public:
	explicit sprite_functor(
		sprite_object &_sprite
	)
	:
		sprite_(_sprite)
	{}

	void
	operator()(
		sge::input::mouse::axis_event const &k
	) const
	{
		switch (k.axis())
		{
		case sge::input::mouse::axis::x:
			sprite_.x(
				static_cast<sprite_object::unit>(sprite_.x() + k.axis_value())
			);

			break;
		case sge::input::mouse::axis::y:
			sprite_.y(
				static_cast<sprite_object::unit>(sprite_.y() + k.axis_value())
			);

			break;
		default:
			break;
		}
	}
private:
	sprite_object &sprite_;
};

typedef sge::sprite::system<
	sprite_choices
>::type sprite_system;

typedef sge::sprite::parameters<
	sprite_choices
> sprite_parameters;


typedef
std::vector<sprite_object>
sprite_vector;

void
console_callback(
	sge::input::keyboard::key_event const &e,
	sge::console::gfx &console_gfx_)
{
	if (!e.pressed())
		return;

	switch (e.key_code())
	{
		case sge::input::keyboard::key_code::f1:
			console_gfx_.active(
				!console_gfx_.active());
			break;
		default:
			break;
	}
}

void
render_callback(
	sprite_system &ss,
	sprite_vector &sprites)
{
	ss.render(
		sprites.begin(),
		sprites.end(),
		sge::sprite::default_sort(),
		sge::sprite::default_equal());
}

void
toggle_filter(
	fruitcut::pp::system &postprocessing,
	sge::console::arg_list const &args,
	sge::console::object &obj)
{
	if (args.size() != 2)
	{
		obj.emit_error(
			SGE_FONT_TEXT_LIT("Expected one argument, the name of the filter"));
		return;
	}

	postprocessing.toggle_filter(
		sge::font::text::to_fcppt_string(
			args[1]));
}

void
list_filters(
	fruitcut::pp::system &postprocessing,
	sge::console::arg_list const &,
	sge::console::object &obj)
{
	BOOST_FOREACH(
		fcppt::string const &s,
		postprocessing.filter_names())
		obj.emit_message(
			sge::font::text::from_fcppt_string(
				s));
}
}

int main()
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug
	);

	sge::renderer::screen_size const screen_size(
		1024,
		768
	);

	sge::systems::instance sys(
		sge::systems::list()
		(
			sge::systems::window(
				sge::renderer::window_parameters(
					FCPPT_TEXT("sge dopplertest")
				)
			)
		)
		(
			sge::systems::renderer(
				sge::renderer::parameters(
					sge::renderer::display_mode(
						screen_size,
						sge::renderer::bit_depth::depth32,
						sge::renderer::refresh_rate_dont_care
					),
					sge::renderer::depth_buffer::off,
					sge::renderer::stencil_buffer::off,
					sge::renderer::window_mode::windowed,
					sge::renderer::vsync::on,
					sge::renderer::no_multi_sampling
				),
				sge::systems::viewport::manage_resize()
			)
		)
		(
			sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector) |
				sge::systems::input_helper::mouse_collector,
				sge::systems::cursor_option_field()
			)
		)
		(sge::systems::parameterless::font)
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				fcppt::assign::make_container<
					sge::extension_set
				>(
					FCPPT_TEXT("png")
				)
			)
		)
	);

	sge::console::object console_object(
		FCPPT_TEXT('/'));

	sge::console::gfx console_gfx(
		console_object,
		sys.renderer(),
		sge::image::colors::black(),
		sys.font_system()->create_font(
			fruitcut::media_path()
			/ FCPPT_TEXT("fonts")
			/ FCPPT_TEXT("default.ttf"),
			static_cast<sge::font::size_type>(
				15)),
		*sys.keyboard_collector(),
		sge::console::sprite_object(
      sge::console::sprite_parameters()
      .texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						sys.renderer()->create_texture(
							sys.image_loader().load(
								fruitcut::media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("console_back.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
      .pos(
        sge::console::sprite_object::point::null())
      .size(
        sge::console::sprite_object::dim(
					static_cast<sge::console::sprite_object::unit>(
						sys.renderer()->screen_size().w()),
          static_cast<sge::console::sprite_object::unit>(
            sys.renderer()->screen_size().h() / 2)))
      .elements()),
		static_cast<sge::console::output_line_limit>(
			1000));

	fcppt::signal::scoped_connection console_connection(
		sys.keyboard_collector()->key_callback(
			boost::bind(
				&console_callback,
				_1,
				boost::ref(
					console_gfx))));

	sge::image2d::file_ptr const
		image_bg(
			sys.image_loader().load(
				fruitcut::media_path()
				/ FCPPT_TEXT("textures")/FCPPT_TEXT("bloom_test.png")
			)
		),
		image_vectorer(
			sys.image_loader().load(
				sge::config::media_path()
				/ FCPPT_TEXT("gui")
				/ FCPPT_TEXT("cursor.png")
			)
		),
		image_tux(
			sys.image_loader().load(
				sge::config::media_path()
				/ FCPPT_TEXT("tux.png")
			)
		);

	sge::texture::manager tex_man(
		sys.renderer(),
		boost::phoenix::construct<
			sge::texture::fragmented_unique_ptr
		>(
			boost::phoenix::new_<
				sge::texture::no_fragmented
			>(
				sys.renderer(),
				sge::image::color::format::rgba8,
				sge::renderer::filter::linear
			)
		)
	);

	sge::texture::const_part_ptr const
		tex_bg(
			sge::texture::add_image(
				tex_man,
				image_bg
			)
		),
		tex_vectorer(
			sge::texture::add_image(
				tex_man,
				image_vectorer
			)
		),
		tex_tux(
			sge::texture::add_image(
				tex_man,
				image_tux
			)
		);

	sprite_system ss(
		sys.renderer()
	);

	sprite_object bg(
		sprite_parameters()
		.texture(
			tex_bg
		)
		.pos(
			sprite_object::point::null()
		)
		.size(
			fcppt::math::dim::structure_cast<
				sprite_object::dim
			>(
				screen_size
			)
		)
		.depth(
			static_cast<
				sprite_object::depth_type
			>(-2)
		)
		.default_color()
		.elements()
	);

	sprite_object vectorer(
		sprite_parameters()
		.texture(
			tex_vectorer
		)
		.depth(
			static_cast<
				sprite_object::depth_type
			>(0)
		)
		.pos(
			sprite_object::point::null()
		)
		.default_color()
		.texture_size()
		.elements()
	);

	sprite_object tux(
		sprite_parameters()
		.pos(
			sprite_object::point(
				static_cast<sprite_object::unit>(screen_size.w()/2-16),
				static_cast<sprite_object::unit>(screen_size.h()/2-16)
			)
		)
		.texture(
			tex_tux
		)
		.size(
			sprite_object::dim(32,32)
		)
		.color(
			sge::image::color::rgba8(
				(sge::image::color::init::red %= 1.0)
				(sge::image::color::init::green %= 1.0)
				(sge::image::color::init::blue %= 1.0)
				(sge::image::color::init::alpha %= 1.0)
			)
		)
		.depth(
			static_cast<
				sprite_object::depth_type
			>(1)
		)
		.elements()
	);

	bool running = true;

	fcppt::signal::scoped_connection const cb(
		sys.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				boost::phoenix::ref(running) = false)));

	fcppt::signal::scoped_connection const pc(
		sys.mouse_collector()->axis_callback(
			::sprite_functor(
				vectorer)));

	sys.renderer()->state(
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = sge::image::colors::red()));

	std::vector<sprite_object> sprites;

	fruitcut::pp::system postprocessing(
		sys.renderer());

	fruitcut::pp::texture::manager texture_manager(
		sys.renderer());

	// This is the data source
	fruitcut::pp::filter::render_to_texture rtt_filter(
		sys.renderer(),
		texture_manager,
		fcppt::math::dim::structure_cast<sge::renderer::dim2>(
			sys.renderer()->screen_size()),
		boost::bind(
			&render_callback,
			boost::ref(
				ss),
			boost::ref(
				sprites)));

	fruitcut::pp::filter::ssaa ssaa_filter(
		sys.renderer(),
		texture_manager,
		fcppt::math::dim::structure_cast<sge::renderer::dim2>(
			sys.renderer()->screen_size()));

	fruitcut::pp::filter::highlight highlight_filter(
		sys.renderer(),
		texture_manager,
		sge::renderer::dim2(
			512,
			512),
		static_cast<sge::renderer::scalar>(0.7));

	fruitcut::pp::filter::blur blur_filter(
		sys.renderer(),
		texture_manager,
		sge::renderer::dim2(
			512,
			512),
		3);

	fruitcut::pp::filter::add add_filter(
		sys.renderer(),
		texture_manager,
		fcppt::math::dim::structure_cast<sge::renderer::dim2>(
			sys.renderer()->screen_size()));

	postprocessing.add_filter(
		rtt_filter,
		FCPPT_TEXT("the_source"),
		fruitcut::pp::dependency_set());

	postprocessing.add_filter(
		ssaa_filter,
		FCPPT_TEXT("ssaa"),
		fcppt::assign::make_container<fruitcut::pp::dependency_set>
			("the_source"));

	postprocessing.add_filter(
		highlight_filter,
		FCPPT_TEXT("highlight"),
		fcppt::assign::make_container<fruitcut::pp::dependency_set>
			("ssaa"));

	postprocessing.add_filter(
		blur_filter,
		FCPPT_TEXT("blur"),
		fcppt::assign::make_container<fruitcut::pp::dependency_set>
			("highlight"));

	postprocessing.add_filter(
		add_filter,
		FCPPT_TEXT("add"),
		fcppt::assign::make_container<fruitcut::pp::dependency_set>
			("ssaa")
			("blur"));

	fcppt::signal::scoped_connection list_filter_connection(
		console_object.insert(
			SGE_FONT_TEXT_LIT("list_filters"),
			boost::bind(
				&list_filters,
				boost::ref(
					postprocessing),
				_1,
				_2),
			SGE_FONT_TEXT_LIT("List all filters")));

	fcppt::signal::scoped_connection pipeline_connection(
		console_object.insert(
			SGE_FONT_TEXT_LIT("toggle_filter"),
			boost::bind(
				&toggle_filter,
				boost::ref(
					postprocessing),
				_1,
				_2),
			SGE_FONT_TEXT_LIT("Toggles a given filter on/off")));

	while(running)
	{
		sys.window()->dispatch();

		sprites.clear();
		sprites.push_back(
			bg);
		sprites.push_back(
			vectorer);
		sprites.push_back(
			tux);

		postprocessing.update();

		sge::renderer::scoped_block scoped_block(
			sys.renderer());

		postprocessing.render_result();

		if (!console_gfx.active())
			continue;

		console_gfx.draw();
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
	std::cerr << e.what() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}

#include "../../fruitlib/rectangle_manager/object.hpp"
#include "../../fruitlib/rectangle_manager/rectangle_instance.hpp"
#include "../../fruitlib/rectangle_manager/padding.hpp"
#include <sge/config/media_path.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image/colors.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/renderer/display_mode.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/sprite/choices.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/sprite/no_color.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/system.hpp>
#include <sge/sprite/with_repetition.hpp>
#include <sge/sprite/with_rotation.hpp>
#include <sge/sprite/with_texture.hpp>
#include <sge/sprite/with_dim.hpp>
#include <sge/sprite/type_choices.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>
#include <sge/sprite/intrusive/tag.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/parameterless.hpp>
#include <sge/systems/running_to_false.hpp>
#include <sge/viewport/center_on_resize.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/no_fragmented.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/part_fwd.hpp>
#include <sge/window/instance.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/chrono/duration.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <exception>
#include <ostream>
#include <cmath>
#include <cstdlib>

namespace
{
typedef
float
float_type;

typedef 
sge::sprite::choices
<
	sge::sprite::type_choices
	<
		float_type,
		float_type,
		sge::image::color::rgba8_format
	>,
	boost::mpl::vector3
	<
		sge::sprite::with_dim,
		sge::sprite::with_color,
		sge::sprite::intrusive::tag
	>
> 
sprite_choices;

typedef 
sge::sprite::system<sprite_choices>::type 
sprite_system;

typedef 
sge::sprite::object<sprite_choices> 
sprite_object;

typedef 
sge::sprite::parameters<sprite_choices> 
sprite_parameters;

typedef
fruitcut::fruitlib::rectangle_manager::object<float_type>
rectangle_manager_type;

typedef
fruitcut::fruitlib::rectangle_manager::rectangle_instance<float_type>
rectangle_instance_type;

typedef
fruitcut::fruitlib::rectangle_manager::padding<float_type>
padding_type;

sge::image::color::rgba8 const
status_fraction_to_color(
	float_type const f)
{
	return 
		sge::image::color::rgba8(
			(sge::image::color::init::red %= 1.0)
			(sge::image::color::init::green %= 1.0)
			(sge::image::color::init::blue %= 1.0)
			(sge::image::color::init::alpha %= (1.0f - std::abs(f))));
}

class rectangle
{
FCPPT_NONCOPYABLE(
	rectangle);
public:
	explicit
	rectangle(
		sprite_system &_sprite_system,
		rectangle_manager_type &_manager,
		rectangle_manager_type::dim const &_size)
	:
		rect_instance_(
			_manager,
			_size),
		sprite_instance_(
			sprite_parameters()
			.pos(
				rect_instance_.bounds().pos())
			.size(
				_size)
			.system(
				&_sprite_system)
			.color(
				status_fraction_to_color(
					rect_instance_.status_fraction()))
			.elements())
	{
	}
	
	void
	update()
	{
		sprite_instance_.pos(
			rect_instance_.bounds().pos());
		sprite_instance_.color(
			status_fraction_to_color(
				rect_instance_.status_fraction()));
	}

	rectangle_instance_type &
	rectangle_instance()
	{
		return rect_instance_;
	}

	rectangle_instance_type const &
	rectangle_instance() const
	{
		return rect_instance_;
	}
private:
	rectangle_instance_type rect_instance_;
	sprite_object sprite_instance_;
};

class manager
{
FCPPT_NONCOPYABLE(
	manager);
public:
	explicit
	manager(
		sge::renderer::device &_renderer)
	:
		frame_timer_(
			sge::time::second(
				1)),
		sprite_system_(
			_renderer),
		rectangle_manager_(
			rectangle_manager_type::rect(
				rectangle_manager_type::vector::null(),
				rectangle_manager_type::dim(
					1024,
					768)),
			padding_type(
				10.0f,
				10.0f,
				10.0f),
			10.0f),
		list_(),
		update_timer_(
			sge::time::second(3))
	{
		list_.push_back(
			new rectangle(
				sprite_system_,
				rectangle_manager_,
				rectangle_manager_type::dim(
					200,
					50)));

		list_.push_back(
			new rectangle(
				sprite_system_,
				rectangle_manager_,
				rectangle_manager_type::dim(
					200,
					200)));
	}

	void
	update()
	{
		rectangle_manager_.update(
			rectangle_manager_type::duration(
				static_cast<float_type>(
					frame_timer_.reset())));

		float_type const epsilon = 
			0.01f;

		for(
			rectangle_list::iterator it = 
				list_.begin();
			it != list_.end();)
		{
			it->update();
			if(std::abs(it->rectangle_instance().status_fraction() - 1.0f) < epsilon)
			{
				std::cout 
					<< "Distance is " 
					<< (std::abs(it->rectangle_instance().status_fraction() + 1.0f)) 
					<< ", erasing\n";
				list_.erase(
					it);
			}
			else
				++it;
		}

		static bool killed = false;
		if(update_timer_.expired() && !killed)
		{
			std::cout << "timer expired, killing\n";
			list_.front().rectangle_instance().kill();
			killed = true;
		}
	}

	void
	render()
	{
		sprite_system_.render_all(
			sge::sprite::default_equal());
	}
private:
	typedef
	boost::ptr_list<rectangle>
	rectangle_list;

	sge::time::timer frame_timer_;
	sprite_system sprite_system_;
	rectangle_manager_type rectangle_manager_;
	rectangle_list list_;
	sge::time::timer update_timer_;
};
}

int main()
try
{
	sge::window::dim const window_dim(
		1024,
		768);

	sge::systems::instance sys(
		sge::systems::list()
		(sge::systems::window(
				sge::window::simple_parameters(
					FCPPT_TEXT("fruitcut rectangle manager test"),
					window_dim)))
		(sge::systems::renderer(
				sge::renderer::parameters(
					sge::renderer::visual_depth::depth32,
					sge::renderer::depth_stencil_buffer::off,
					sge::renderer::vsync::on,
					sge::renderer::no_multi_sampling),
				sge::viewport::center_on_resize(
					window_dim)))
		(sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector),
				sge::systems::cursor_option_field::null())));

	manager rect_manager(
		sys.renderer());
	
	bool running = true;

	fcppt::signal::scoped_connection const cb(
		sys.keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				sge::systems::running_to_false(
					running))));

	sys.renderer().state(
		sge::renderer::state::list 
			(sge::renderer::state::bool_::clear_back_buffer = true)
			(sge::renderer::state::color::back_buffer_clear_color =
				sge::image::colors::black()));

	while(
		running)
	{
		sys.window().dispatch();

		sge::renderer::scoped_block const block(
			sys.renderer());

		rect_manager.update();
		rect_manager.render();
	}
}
 catch(
	fcppt::exception const &_error)
{
	fcppt::io::cerr
		<< _error.string()
		<< FCPPT_TEXT('\n');
	
	return EXIT_FAILURE;
}
 catch(
	std::exception const &_error)
{
	std::cerr
		<< _error.what()
		<< '\n';
	
	return EXIT_FAILURE;
}

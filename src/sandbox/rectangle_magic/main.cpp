#include <sge/renderer/clear/parameters.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <fruitlib/rectangle_manager/object.hpp>
#include <fruitlib/rectangle_manager/padding.hpp>
#include <fruitlib/rectangle_manager/rectangle_instance.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <sge/renderer/bit_depth.hpp>
#include <sge/renderer/depth_stencil_buffer.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/display_mode.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/vsync.hpp>
#include <sge/renderer/windowed.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/buffers/option.hpp>
#include <sge/sprite/buffers/single_impl.hpp>
#include <sge/sprite/buffers/with_declaration_impl.hpp>
#include <sge/sprite/compare/default.hpp>
#include <sge/sprite/config/choices.hpp>
#include <sge/sprite/config/float_type.hpp>
#include <sge/sprite/config/intrusive.hpp>
#include <sge/sprite/config/normal_size.hpp>
#include <sge/sprite/config/type_choices.hpp>
#include <sge/sprite/config/unit_type.hpp>
#include <sge/sprite/config/with_color.hpp>
#include <sge/sprite/intrusive/collection.hpp>
#include <sge/sprite/process/all.hpp>
#include <sge/systems/cursor_option_field.hpp>
#include <sge/systems/input.hpp>
#include <sge/systems/input_helper.hpp>
#include <sge/systems/input_helper_field.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/quit_on_escape.hpp>
#include <sge/systems/renderer.hpp>
#include <sge/systems/window.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/elapsed_fractional_and_reset.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <sge/viewport/center_on_resize.hpp>
#include <sge/window/dim.hpp>
#include <sge/window/parameters.hpp>
#include <sge/window/system.hpp>
#include <sge/window/title.hpp>
#include <awl/main/exit_code.hpp>
#include <awl/main/exit_failure.hpp>
#include <awl/main/function_context_fwd.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fcppt/container/ptr/insert_unique_ptr.hpp>
#include <fcppt/container/ptr/push_front_unique_ptr.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/random/variate.hpp>
#include <fcppt/random/distribution/uniform_int.hpp>
#include <fcppt/random/distribution/uniform_real.hpp>
#include <fcppt/random/generator/minstd_rand.hpp>
#include <fcppt/random/generator/seed_from_chrono.hpp>
#include <fcppt/signal/connection.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <cmath>
#include <exception>
#include <iterator>
#include <ostream>
#include <fcppt/config/external_end.hpp>


namespace
{
typedef
float
float_type;

typedef
sge::sprite::config::choices
<
	sge::sprite::config::type_choices
	<
		sge::sprite::config::unit_type
		<
			float_type
		>,
		sge::sprite::config::float_type
		<
			float_type
		>
	>,
	sge::sprite::config::normal_size,
	boost::mpl::vector2
	<
		sge::sprite::config::with_color
		<
			sge::image::color::rgba8_format
		>,
		sge::sprite::config::intrusive
	>
>
sprite_choices;

typedef
sge::sprite::buffers::with_declaration
<
	sge::sprite::buffers::single
	<
		sprite_choices
	>
>
sprite_buffers;

typedef
sge::sprite::intrusive::collection<sprite_choices>
sprite_collection;

typedef
sge::sprite::object<sprite_choices>
sprite_object;

typedef
sge::sprite::parameters<sprite_choices>
sprite_parameters;

typedef
fruitlib::rectangle_manager::object<float_type>
rectangle_manager_type;

typedef
fruitlib::rectangle_manager::rectangle_instance<float_type>
rectangle_instance_type;

typedef
fruitlib::rectangle_manager::padding<float_type>
padding_type;

sge::image::color::rgba8 const
status_fraction_to_color(
	float_type const f)
{
	return
		sge::image::color::rgba8(
			(sge::image::color::init::red() %= 1.0)
			(sge::image::color::init::green() %= 1.0)
			(sge::image::color::init::blue() %= 1.0)
			(sge::image::color::init::alpha() %= (1.0f - std::abs(f))));
}

class rectangle
{
FCPPT_NONCOPYABLE(
	rectangle);
public:
	explicit
	rectangle(
		sprite_collection &_sprite_collection,
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
			.connection(
				_sprite_collection.connection())
			.color(
				status_fraction_to_color(
					rect_instance_.status_fraction())))
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
			sge::timer::parameters<sge::timer::clocks::standard>(
				boost::chrono::seconds(
					1))),
		sprite_buffers_(
			_renderer,
			sge::sprite::buffers::option::dynamic),
		sprite_collection_(),
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
		number_generator_(
			fcppt::random::generator::seed_from_chrono<random_generator_type::seed>()),
		update_rng_(
			number_generator_,
			uniform_float_distribution(
				uniform_float_distribution::min(
					float_type(
						0.1f)),
				uniform_float_distribution::sup(
					float_type(
						0.5f)))),
		action_rng_(
			number_generator_,
			uniform_int_distribution(
				uniform_int_distribution::min(
					0),
				uniform_int_distribution::max(
					1))),
		create_size_w_rng_(
			number_generator_,
			uniform_float_distribution(
				uniform_float_distribution::min(
					float_type(
						50.0f)),
				uniform_float_distribution::sup(
					float_type(
						500.0f)))),
		create_size_h_rng_(
			number_generator_,
			uniform_float_distribution(
				uniform_float_distribution::min(
					float_type(
						10.0f)),
				uniform_float_distribution::sup(
					float_type(
						100.0f)))),
		update_timer_(
			sge::timer::parameters<sge::timer::clocks::standard>(
				boost::chrono::duration<float_type>(
					update_rng_())))
	{
	}

	void
	update()
	{
		rectangle_manager_.update(
			rectangle_manager_type::duration(
				sge::timer::elapsed_fractional_and_reset<float_type>(
					frame_timer_)));

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
				/*
				std::cout
					<< "Finally erasing a rectangle\n";
				*/
				it =
					list_.erase(
						it);
			}
			else
				++it;
		}

		if(sge::timer::reset_when_expired(update_timer_))
		{
			if(all_dead() || action_rng_())
				action_create();
			else
				action_kill();
		}
	}

	void
	render()
	{
		sge::sprite::process::all(
			sprite_collection_.range(),
			sprite_buffers_,
			sge::sprite::compare::default_());
	}
private:
	typedef
	boost::ptr_list<rectangle>
	rectangle_list;

	sge::timer::basic<sge::timer::clocks::standard> frame_timer_;
	sprite_buffers sprite_buffers_;
	sprite_collection sprite_collection_;
	rectangle_manager_type rectangle_manager_;
	rectangle_list list_;

	typedef
	fcppt::random::generator::minstd_rand
	random_generator_type;

	typedef
	fcppt::random::distribution::uniform_real<float_type>
	uniform_float_distribution;

	typedef
	fcppt::random::distribution::uniform_int<int>
	uniform_int_distribution;

	typedef
	fcppt::random::variate
	<
		random_generator_type,
		uniform_float_distribution
	>
	uniform_float_variate;

	typedef
	fcppt::random::variate
	<
		random_generator_type,
		uniform_int_distribution
	>
	uniform_int_variate;

	random_generator_type number_generator_;
	uniform_float_variate update_rng_;
	uniform_int_variate action_rng_;
	uniform_float_variate create_size_w_rng_;
	uniform_float_variate create_size_h_rng_;
	sge::timer::basic<sge::timer::clocks::standard> update_timer_;

	void
	action_create()
	{
		std::cout << "Creating a new rectangle\n";
		fcppt::container::ptr::push_front_unique_ptr(
			list_,
			fcppt::make_unique_ptr<rectangle>(
				fcppt::ref(
					sprite_collection_),
				fcppt::ref(
					rectangle_manager_),
				rectangle_manager_type::dim(
					create_size_w_rng_(),
					create_size_h_rng_())));
	}

	void
	action_kill()
	{
		/*
		std::cout
			<< "Killing a rectangle, all dead is "
			<< all_dead()
			<< ", number of elements: "
			<< list_.size()
			<< "\n";
		*/

		typedef
		std::iterator_traits<rectangle_list::iterator>::difference_type
		difference_type;

		typedef
		fcppt::random::distribution::uniform_int<difference_type>
		uniform_difference_type_distribution;

		typedef
		fcppt::random::variate
		<
			random_generator_type,
			uniform_difference_type_distribution
		>
		uniform_difference_type_variate;

		/*
		std::cout << "First dead element: " <<
			std::distance(
				list_.begin(),
				this->first_dead_element()) << "\n";
		*/

		uniform_difference_type_variate element_position_rng(
			number_generator_,
			uniform_difference_type_distribution(
				uniform_difference_type_distribution::min(
					0),
				uniform_difference_type_distribution::max(
					std::distance(
						list_.begin(),
						this->first_dead_element())-1)));

		difference_type const to_kill_index =
			element_position_rng();

	//	std::cout << "To kill index: " << to_kill_index << "\n";

		std::cout << '|';
		for(difference_type i = 0; i < static_cast<difference_type>(list_.size()); ++i)
		{
			if(i == to_kill_index)
				std::cout << '#';
			else if (i == std::distance(list_.begin(),first_dead_element()))
				std::cout << 'f';
			else if (boost::next(list_.begin(),i)->rectangle_instance().killed())
				std::cout << 'd';
			else
				std::cout << '-';
		}
		std::cout << "|\n";

		rectangle_list::iterator to_kill =
			boost::next(
				list_.begin(),
				to_kill_index);

		to_kill->rectangle_instance().kill();

		fcppt::unique_ptr<rectangle> released_element(
			list_.release(
				to_kill).release());

		fcppt::container::ptr::insert_unique_ptr(
			list_,
			this->first_dead_element(),
			fcppt::move(
				released_element));
	}

	rectangle_list::iterator const
	first_dead_element()
	{
		for(rectangle_list::iterator i = list_.begin(); i != list_.end(); ++i)
			if(i->rectangle_instance().killed())
			{
				for(rectangle_list::iterator j = i; j != list_.end(); ++j)
				{
					if(!j->rectangle_instance().killed())
						throw std::runtime_error("Fuck mann!");
				}
				return i;
			}
		return list_.end();
	}

	bool
	all_dead() const
	{
		if(list_.empty())
			return true;
		for(rectangle_list::const_iterator i = list_.begin(); i != list_.end(); ++i)
			if(!i->rectangle_instance().killed())
				return false;
		return true;
	}
};
}

awl::main::exit_code const
rectangle_main(
	awl::main::function_context const &);

awl::main::exit_code const
rectangle_main(
	awl::main::function_context const &)
try
{
	sge::window::dim const window_dim(
		1024,
		768);

	sge::systems::instance sys(
		sge::systems::list()
		(sge::systems::window(
				sge::window::parameters(
					sge::window::title(
						FCPPT_TEXT("fruitcut rectangle manager test")),
					window_dim)))
		(sge::systems::renderer(
				sge::renderer::parameters(
					sge::renderer::windowed(
						sge::renderer::bit_depth::depth32),
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

	fcppt::signal::scoped_connection const escape_connection(
		sge::systems::quit_on_escape(
			sys));

	while(
		sys.window_system().poll())
	{
		sys.renderer().onscreen_target().clear(
			sge::renderer::clear::parameters()
				.back_buffer(
					sge::image::colors::black())
				.depth_buffer(
					sge::renderer::clear::depth_buffer_value(
						1.0f)));

		sge::renderer::scoped_block const block(
			sys.renderer());

		rect_manager.update();
		rect_manager.render();
	}

	return
		sys.window_system().exit_code();
}
catch(
	fcppt::exception const &_error)
{
	fcppt::io::cerr()
		<< _error.string()
		<< FCPPT_TEXT('\n');

	return awl::main::exit_failure();
}
catch(
	std::exception const &_error)
{
	std::cerr
		<< _error.what()
		<< '\n';

	return awl::main::exit_failure();
}

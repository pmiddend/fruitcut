#ifndef FRUITAPP_FRUIT_SPAWNER_HPP_INCLUDED
#define FRUITAPP_FRUIT_SPAWNER_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/fruit/manager_fwd.hpp>
#include <fruitapp/fruit/prototype_sequence.hpp>
#include <fruitlib/random_generator_fwd.hpp>
#include <fruitlib/uniform_int_random.hpp>
#include <fruitlib/uniform_real_random.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/camera/first_person/object_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
class spawner
:
	public fruitlib::scenic::node<spawner>
{
FCPPT_NONCOPYABLE(
	spawner);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	// This should have more info, for example the position of the
	// spawned fruit, but there you go.
	typedef
	void spawn_callback_fn();

	typedef
	fcppt::function::object<spawn_callback_fn>
	spawn_callback_function;

	explicit
	spawner(
		fruitlib::scenic::optional_parent const &,
		fruit::manager &,
		fruitlib::random_generator &,
		sge::parse::json::object const &config_file,
		sge::camera::first_person::object const &,
		fruitapp::ingame_clock const &);

	fcppt::signal::auto_connection
	spawn_callback(
		spawn_callback_function const &);

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	typedef
	fruitlib::uniform_real_random<fruitapp::ingame_clock::float_type>::type
	uniform_float_variate;

	typedef
	fruitlib::uniform_real_random<fruitlib::physics::scalar>::type
	uniform_physics_variate;

	typedef
	fruitlib::uniform_int_random<prototype_sequence::size_type>::type
	uniform_prototype_size_variate;

	fruit::manager &manager_;
	sge::camera::first_person::object const &camera_;
	uniform_float_variate seconds_rng_;
	uniform_prototype_size_variate prototype_rng_;
	// x for x coordinate. This is a scalar rather than
	// renderer::pixel_unit because the viewport might change and I
	// don't want to reinitialize the rng then. A value in [0,1] suffices
	uniform_physics_variate x_rng_;
	uniform_physics_variate linear_velocity_rng_;
	uniform_physics_variate angular_velocity_rng_;
	uniform_physics_variate angle_rng_;
	fruitapp::ingame_timer timer_;
	fcppt::signal::object<spawn_callback_fn> spawn_signal_;

	void
	reset_timer();
};
}
}

#endif

#ifndef FRUITAPP_FRUIT_SPAWNER_HPP_INCLUDED
#define FRUITAPP_FRUIT_SPAWNER_HPP_INCLUDED

#include <fruitapp/fruit/prototype_sequence.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/uniform_random.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update_fwd.hpp>
#include <fruitlib/random_generator.hpp>
#include <fruitapp/fruit/manager_fwd.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/timer.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <boost/mpl/vector/vector10.hpp>

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
		sge::camera::object const &,
		sge::time::callback const &);

	fcppt::signal::auto_connection
	spawn_callback(
		spawn_callback_function const &);

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	fruit::manager &manager_;
	sge::camera::object const &camera_;
	fruitlib::uniform_random<sge::time::funit>::type seconds_rng_;
	fruitlib::uniform_random<prototype_sequence::size_type>::type prototype_rng_;
	// x for x coordinate. This is a scalar rather than
	// renderer::pixel_unit because the viewport might change and I
	// don't want to reinitialize the rng then. A value in [0,1] suffices
	fruitlib::uniform_random<fruitlib::physics::scalar>::type x_rng_;
	fruitlib::uniform_random<fruitlib::physics::scalar>::type linear_velocity_rng_;
	fruitlib::uniform_random<fruitlib::physics::scalar>::type angular_velocity_rng_;
	fruitlib::uniform_random<fruitlib::physics::scalar>::type angle_rng_;
	sge::time::timer timer_;
	fcppt::signal::object<spawn_callback_fn> spawn_signal_;

	void
	reset_timer();
};
}
}

#endif

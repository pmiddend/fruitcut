#ifndef FRUITCUT_APP_FRUIT_SPAWNER_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_SPAWNER_HPP_INCLUDED

#include "prototype_sequence.hpp"
#include "../../physics/scalar.hpp"
#include "../../uniform_random.hpp"
#include "../../scenic/nodes/intrusive.hpp"
#include "manager_fwd.hpp"
#include <sge/camera/object_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/timer.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
class spawner
:
	public scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	spawner);
public:
	// This should have more info, for example the position of the
	// spawned fruit, but there you go.
	typedef
	void spawn_callback_fn();

	typedef
	std::tr1::function<spawn_callback_fn>
	spawn_callback_function;

	explicit
	spawner(
		manager &,
		sge::parse::json::object const &config_file,
		sge::camera::object const &,
		sge::time::callback const &);

	fcppt::signal::auto_connection
	spawn_callback(
		spawn_callback_function const &);
private:
	manager &manager_;
	sge::camera::object const &camera_;
	uniform_random<sge::time::funit>::type seconds_rng_;
	uniform_random<prototype_sequence::size_type>::type prototype_rng_;
	// x for x coordinate. This is a scalar rather than
	// renderer::pixel_unit because the viewport might change and I
	// don't want to reinitialize the rng then. A value in [0,1] suffices
	uniform_random<physics::scalar>::type x_rng_;
	uniform_random<physics::scalar>::type linear_velocity_rng_;
	uniform_random<physics::scalar>::type angular_velocity_rng_;
	uniform_random<physics::scalar>::type angle_rng_;
	sge::time::timer timer_;
	fcppt::signal::object<spawn_callback_fn> spawn_signal_;

	void
	reset_timer();

	void
	update();

	void
	render();
};
}
}
}

#endif

#ifndef FRUITCUT_APP_FRUIT_SPAWNER_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_SPAWNER_HPP_INCLUDED

#include "prototype_sequence.hpp"
#include "../../physics/scalar.hpp"
#include "../../uniform_random.hpp"
#include "manager_fwd.hpp"
#include <sge/camera/object_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/timer.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
class spawner
{
FCPPT_NONCOPYABLE(
	spawner);
public:
	explicit
	spawner(
		manager &,
		sge::parse::json::object const &config_file,
		sge::camera::object const &,
		sge::time::callback const &);

	void
	update();
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

	void
	reset_timer();
};
}
}
}

#endif

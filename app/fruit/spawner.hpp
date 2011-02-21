#ifndef FRUITCUT_APP_FRUIT_SPAWNER_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_SPAWNER_HPP_INCLUDED

#include "../../physics/vector3.hpp"
#include <sge/camera/object_fwd.hpp>
#include <sge/time/callback.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/function.hpp>

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
	// linear velocity, angular velocity
	typedef
	boost::function<void (physics::vector3 const &,physics::vector3 const &)>
	spawn_callback;

	explicit
	spawner(
		sge::camera::object const &,
		sge::time::callback const &,
		spawn_callback const &);

	void
	update();
private:
	sge::camera::object const &camera_;
	// sge::time::timer has no callback getter, so we have to store it
	// here
	sge::time::callback const callback_;
	spawn_callback spawn_callback_;
};
}
}
}

#endif

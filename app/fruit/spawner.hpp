#ifndef FRUITCUT_APP_FRUIT_SPAWNER_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_SPAWNER_HPP_INCLUDED

#include "manager_fwd.hpp"
#include <sge/camera/object_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/timer.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/random/uniform.hpp>

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
	typedef
	fcppt::random::uniform<sge::time::funit>
	funit_uniform;

	manager &manager_;
	sge::camera::object const &camera_;
	// sge::time::timer has no callback getter, so we have to store it
	// here
	//sge::time::callback const callback_;
	funit_uniform seconds_rng_;
	sge::time::timer timer_;

	void
	reset_timer();
};
}
}
}

#endif

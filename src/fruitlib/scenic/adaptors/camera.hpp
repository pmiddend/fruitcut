#ifndef FRUITLIB_SCENIC_ADAPTORS_CAMERA_HPP_INCLUDED
#define FRUITLIB_SCENIC_ADAPTORS_CAMERA_HPP_INCLUDED

#include <fruitlib/scenic/events/update_fwd.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/timer.hpp>
#include <fruitlib/scenic/delta/clock.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <sge/camera/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitlib
{
namespace scenic
{
namespace adaptors
{
class camera
:
	public scenic::node<camera>
{
FCPPT_NONCOPYABLE(
	camera);
public:
	typedef
	boost::mpl::vector1<scenic::events::update>
	scene_reactions;

	explicit
	camera(
		scenic::optional_parent const &parent,
		sge::camera::object &,
		scenic::delta::callback const &);

	~camera();

	void
	react(
		scenic::events::update const &);
private:
	scenic::delta::clock clock_;
	scenic::delta::timer timer_;
	sge::camera::object &camera_;
};
}
}
}

#endif

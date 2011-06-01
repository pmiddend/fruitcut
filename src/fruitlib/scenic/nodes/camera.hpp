#ifndef FRUITCUT_FRUITLIB_SCENIC_NODES_CAMERA_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_NODES_CAMERA_HPP_INCLUDED

#include "intrusive.hpp"
#include <sge/camera/object_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/timer.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
{
namespace nodes
{
class camera
:
	public virtual intrusive
{
FCPPT_NONCOPYABLE(
	camera);
public:
	explicit
	camera(
		sge::camera::object &,
		sge::time::callback const &);

	~camera();
protected:
	sge::camera::object &object_;
	sge::time::timer timer_;

	void
	update();

	void
	render();
};
}
}
}
}

#endif

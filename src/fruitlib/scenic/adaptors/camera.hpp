#ifndef FRUITCUT_FRUITLIB_SCENIC_ADAPTORS_CAMERA_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_ADAPTORS_CAMERA_HPP_INCLUDED

#include "../events/update_fwd.hpp"
#include "../node.hpp"
#include "../optional_parent.hpp"
#include <sge/camera/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
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
		sge::camera::object &);

	~camera();

	void
	react(
		scenic::events::update const &);
private:
	sge::camera::object &camera_;
};
}
}
}
}

#endif
